#include <QtWidgets>
#include "gamewindow.h"
#include <iostream>
#include <fstream>

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

    filepath = QString("/home/grog/Code-Stuff/AudioPlayer/songs/BK/TootysTheme.mp3");
    player->setSource(QUrl::fromLocalFile(filepath));
    audioOutput->setVolume(1.0);
    player->play();
    readSongs();
}

void GameWindow::nextTrack() {
    //Audio stuff
    player->stop();
    filepath = QString("/home/grog/Code-Stuff/AudioPlayer/songs/BK/Beta1Advent.mp3");
    player->setSource(QUrl::fromLocalFile(filepath));
    audioOutput->setVolume(1.0);
    player->play();
}

void GameWindow::readSongs() {
    std::string songFilePath = projectPath.toStdString() + "/songs/songs.txt";
    setWindowTitle(QString::fromStdString(songFilePath));
    std::ifstream songFile(songFilePath);
    std::string line;

    while (getline(songFile, line)) {
        std::cout << line << std::endl;
    }
    songFile.close();
}