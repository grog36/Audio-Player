#include <QtWidgets>
#include "gamewindow.h"

//Constructor
GameWindow::GameWindow(QWidget* parent) : QWidget(parent) {
    //Start Button
    startButton = new QPushButton();
    startButton->setText("START");
    startButton->setStyleSheet("background-color: green; color: white; font-size: 24px;");

    //Game Picture
    QPixmap* pixmap = new QPixmap("/home/grog/Code-Stuff/AudioPlayer/songs/Conker/Conker.jpg");
    gamePicture = new QLabel();
    gamePicture->setPixmap(*pixmap);

    //Layout Stuff
    QGridLayout* mainLayout = new QGridLayout();
    mainLayout->addWidget(gamePicture, 0, 0);
    mainLayout->addWidget(startButton, 1, 0);
    setLayout(mainLayout);

    //Window Title
    setWindowTitle(tr("Grog's Audio Player"));

    //Signals and Slots
    connect(startButton, SIGNAL(pressed()), this, SLOT(turnOnMusic()));
}

//Destructor
GameWindow::~GameWindow() {
    delete audioOutput;
    delete player;
    delete gamePicture;
    delete startButton;
}

void GameWindow::turnOnMusic() {
    //Audio stuff
    player = new QMediaPlayer();
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    audioOutput->setMuted(false);

    filepath = QString("/home/grog/Code-Stuff/AudioPlayer/songs/Conker/Track8TheUggas.mp3");
    player->setSource(QUrl::fromLocalFile(filepath));
    audioOutput->setVolume(1.0);
    player->play();
}