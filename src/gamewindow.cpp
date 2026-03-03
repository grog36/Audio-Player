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
    QPixmap* pixmap = new QPixmap("/home/grog/Code-Stuff/AudioPlayer/songs/BK/BK.jpg");
    gamePicture = new QLabel();
    gamePicture->setPixmap(*pixmap);

    //Layout Stuff
    QGridLayout* mainLayout = new QGridLayout();
    mainLayout->addWidget(gamePicture, 0, 0);
    mainLayout->addWidget(playPauseButton, 1, 0);
    mainLayout->addWidget(nextButton, 1, 1);
    //Set column & row sizes
    mainLayout->setRowStretch(0, 2);
    mainLayout->setRowStretch(1, 1);
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
}

void GameWindow::turnOnMusic() {
    //Audio stuff
    player = new QMediaPlayer();
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    audioOutput->setMuted(false);
    
    readSongs();
    filepath = QString::fromStdString(getNextSong());
    player->setSource(QUrl::fromLocalFile(filepath));
    player->play();
}

void GameWindow::nextTrack() {
    //Audio stuff
    player->stop();
    filepath = QString::fromStdString(getNextSong());
    player->setSource(QUrl::fromLocalFile(filepath));
    player->play();
    getGame();
}

void GameWindow::readSongs() {
    std::string songFilePath = projectPath.toStdString() + "/songs/songs.txt";
    setWindowTitle(QString::fromStdString(songFilePath));
    std::ifstream songFile(songFilePath);
    std::string line;

    while (getline(songFile, line)) {
        if (line != "") {
            std::string currentSong = projectPath.toStdString() + "/songs/" + line + ".mp3";
            songList.push_back(currentSong);
        }
    }
    songFile.close();

    shuffleQueue();
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

std::string GameWindow::getNextSong() {
    if (songQueue.size() == 0) {
        shuffleQueue();
    }
    std::string nextSong = songQueue.front();
    songQueue.pop();
    return nextSong;
}

//Changes filepath to game picture path
void GameWindow::getGame() {
    qsizetype whereToTruncate = filepath.lastIndexOf("/");
    filepath.truncate(whereToTruncate);
    qsizetype gameNameBeginning = filepath.lastIndexOf("/") + 1;
    QString gameName = filepath.slice(gameNameBeginning);
}