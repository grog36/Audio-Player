#include <QtWidgets>
#include "gamewindow.h"
#include <fstream>
#include <random>
#include <chrono>

//Constructor
GameWindow::GameWindow(QWidget* parent) : QWidget(parent) {
    //Buttons
    playPauseButton = new QPushButton();
    playPauseButton->setText("Play/Pause");
    playPauseButton->setStyleSheet("background-color: green; color: white; font-size: 24px;");
    nextButton = new QPushButton();
    nextButton->setText("Next Track");
    nextButton->setStyleSheet("background-color: pink; color: black; font-size: 24px;");

    //Game Picture
    gamePicture = new QLabel();
    gamePicture->setPixmap(QPixmap("/home/grog/Code-Stuff/AudioPlayer/songs/Banjo-Kazooie/Banjo-Kazooie.jpg"));

    //Track Title
    trackTitle = new QLabel();
    trackTitle->setText("No Track Playing Currently");
    trackTitle->setStyleSheet("color: black; font-size: 32px;");

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
    connect(playPauseButton, SIGNAL(pressed()), this, SLOT(turnOnMusic()));
    connect(nextButton, SIGNAL(pressed()), this, SLOT(nextTrack()));
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

void GameWindow::turnOnMusic() {
    //Setup queue
    readSongs();
    shuffleQueue();

    //Starting playing music
    audioOutput->setMuted(false);
    filepath = QString::fromStdString(getNextSong());
    player->setSource(QUrl::fromLocalFile(filepath));
    player->play();
}

void GameWindow::nextTrack() {
    //Stops the current player, loads in new song, and starts it up again
    player->stop();
    filepath = QString::fromStdString(getNextSong());
    player->setSource(QUrl::fromLocalFile(filepath));
    player->play();
    getGame();
}

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

//Generates a random number from [min, max],
int GameWindow::getRandomNumber(int min, int max) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

//Returns the next song and updates the screen accordingly
std::string GameWindow::getNextSong() {
    if (songQueue.size() == 0) {
        shuffleQueue();
    }
    std::string nextSong = songQueue.front();
    songQueue.pop();
    trackTitle->setText(QString::fromStdString(nextSong));
    return nextSong;
}

//Updates the game picture
void GameWindow::getGame() {
    //Edits the filepath to simplify process
    qsizetype whereToTruncate = filepath.lastIndexOf("/");
    filepath.truncate(whereToTruncate);
    qsizetype startOfDir = filepath.lastIndexOf("/");

    //Gets the game name from the current directory
    std::string gameName;
    for (int i = startOfDir + 1; i < filepath.size(); i++) {
        gameName.push_back(filepath.at(i).toLatin1());
    }

    //Grabs the path of the picture
    QString picturePath;
    picturePath.append(filepath);
    picturePath.append("/");
    picturePath.append(gameName);
    picturePath.append(".jpg");

    //Updates the on-screen picture accordingly
    gamePicture->setPixmap(QPixmap(picturePath));
}