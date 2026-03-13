#include "gamewindow.h" //Header file
#include <QtWidgets> //Necessary widgets
#include <fstream> //File reading
#include <random> //For random number generation
#include <chrono> //For random number generation

//Constructor
GameWindow::GameWindow(QWidget* parent) : QWidget(parent) {
    //Buttons
    playPauseButton = new QPushButton();
    playPauseButton->setText("Start");
    playPauseButton->setStyleSheet("background-color: green; color: white; font-size: 24px; border: 2px solid red;");
    nextButton = new QPushButton();
    nextButton->setText("Next Track");
    nextButton->setStyleSheet("background-color: pink; color: black; font-size: 24px; border: 2px solid red;");

    //Game Picture
    gamePicture = new QLabel();
    gamePicture->setStyleSheet("border: 2px solid red;");

    //Track Title
    trackTitle = new QLabel();
    trackTitle->setText("No Track Playing Currently");
    trackTitle->setStyleSheet("color: black; font-size: 32px; border: 2px solid red;");

    //Audio stuff
    player = new QMediaPlayer();
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);

    //Layout Stuff
    QGridLayout* mainLayout = new QGridLayout();
    mainLayout->addWidget(trackTitle, 0, 0);
    mainLayout->addWidget(gamePicture, 1, 0);
    mainLayout->addWidget(playPauseButton, 2, 0);
    mainLayout->addWidget(nextButton, 2, 1);
    //Set column & row sizes
    mainLayout->setRowStretch(0, 1);
    mainLayout->setRowStretch(1, 2);
    mainLayout->setRowStretch(2, 1);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 1);
    //finally
    setLayout(mainLayout);

    //Window Title
    setWindowTitle(tr("Grog's Audio Player"));

    //Signals and Slots
    connect(playPauseButton, SIGNAL(pressed()), this, SLOT(toggleMusic()));
    connect(nextButton, SIGNAL(pressed()), this, SLOT(nextTrack()));
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(checkForTrackEnding(QMediaPlayer::MediaStatus)));
}

//Destructor
GameWindow::~GameWindow() {
    delete audioOutput;
    delete player;
    delete gamePicture;
    delete playPauseButton;
    delete nextButton;
    delete trackTitle;
}

/**
 * @brief SLOT that performs setup and turns on music
 */
void GameWindow::toggleMusic() {
    //Music needs to initialize
    if (musicState == 0) {
        //Setup queue
        readSongs();
        shuffleQueue();
        //Starting playing music
        audioOutput->setMuted(false);
        filepath = QString::fromStdString(getNextSong());
        player->setSource(QUrl::fromLocalFile(filepath));
        getGame();
        player->play();
        musicState = 2;
        playPauseButton->setText("Pause");
    }
    //Music is currently paused and needs to start again
    else if (musicState == 1) {
        player->play();
        musicState = 2;
        playPauseButton->setText("Pause");
    }
    //Music is currently playing and needs to be paused
    else {
        player->pause();
        musicState = 1;
        playPauseButton->setText("Resume");
    }

}

/**
 * @brief SLOT that stops the previous track and starts the next
 */
void GameWindow::nextTrack() {
    player->stop();
    filepath = QString::fromStdString(getNextSong());
    player->setSource(QUrl::fromLocalFile(filepath));
    player->play();
    musicState = 2;
    getGame();
    playPauseButton->setText("Pause");
}

/**
 * @brief SLOT that calls nextTrack() when current track ends
 */
void GameWindow::checkForTrackEnding(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        nextTrack();
    }
}

/**
 * @brief Reads songs in from "/songs/songs.txt"
 */
void GameWindow::readSongs() {
    std::string songFilePath = projectPath.toStdString() + "/songs/songs.txt";
    std::ifstream songFile(songFilePath);
    std::string line;

    while (getline(songFile, line)) {
        if (line != "") {
            std::string currentSong = projectPath.toStdString() + "/songs/" + line + ".mp3";
            songList.push_back(currentSong);
        }
    }
    songFile.close();
}

/**
 * @brief Shuffles the list of songs and adds them to a queue
 */
void GameWindow::shuffleQueue() {
    //Setup indexes vector
    std::vector<int> indexes;
    for (int i = 0; i < songList.size(); i++) {
        indexes.push_back(i);
    }
    
    for (int i = 0; i < songList.size(); i++) {
        int currentRandomIndex = indexes[getRandomNumber(0, indexes.size() - 1)];
        indexes.erase(indexes.begin() + currentRandomIndex);
        songQueue.push(songList[currentRandomIndex]);
    }
}

/**
 * @brief Generates a random int from min to max inclusive
 *
 * @param min - The minimum value
 * @param max - The maximum value
 * @return A random number from min to max, inclusive
 */
int GameWindow::getRandomNumber(int min, int max) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

/**
 * @brief Gets the next song and updates trackTitle & gameName
 *
 * @return Returns the path of the next song
 */
std::string GameWindow::getNextSong() {
    //Updates the song queue and pops song
    if (songQueue.size() == 0) {
        shuffleQueue();
    }
    std::string nextSongPath = songQueue.front();
    songQueue.pop();

    //Does some substring magic to find and update gameName and songName
    int lastSlashIndex = nextSongPath.find_last_of('/');
    std::string songName = nextSongPath.substr(lastSlashIndex + 1);
    int slashIndexBeforeGame = nextSongPath.find_last_of('/', lastSlashIndex - 1);
    gameName = nextSongPath.substr(slashIndexBeforeGame + 1, (lastSlashIndex - slashIndexBeforeGame - 1));

    //Updates screen accordingly
    trackTitle->setText(QString::fromStdString(gameName) + " : " + QString::fromStdString(songName));

    return nextSongPath;
}

/**
 * @brief Updates the game picture based off of the current game
 */
void GameWindow::getGame() {
    //Edits the filepath to simplify process
    qsizetype whereToTruncate = filepath.lastIndexOf("/");
    filepath.truncate(whereToTruncate);
    qsizetype startOfDir = filepath.lastIndexOf("/");

    //Grabs the path of the picture
    QString picturePath;
    picturePath.append(filepath);
    picturePath.append("/");
    picturePath.append(gameName);
    picturePath.append(".jpg");

    //Updates the on-screen picture accordingly
    gamePicture->setPixmap(QPixmap(picturePath));
}