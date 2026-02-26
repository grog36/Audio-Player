#include <QtWidgets>
#include "gamewindow.h"

//Constructor
GameWindow::GameWindow(QWidget* parent) : QWidget(parent) {
    startButton = new QPushButton();
    startButton->setText("START");
    //Visual stuff
    QGridLayout* mainLayout = new QGridLayout();
    mainLayout->addWidget(startButton, 0, 0);
    setLayout(mainLayout);
    setWindowTitle(tr("Grog's Audio Player"));

    connect(startButton, SIGNAL(pressed()), this, SLOT(turnOnMusic()));
}

//Destructor
GameWindow::~GameWindow() {
    delete audioOutput;
    delete player;
}

void GameWindow::turnOnMusic() {
    //Audio stuff
    player = new QMediaPlayer();
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    audioOutput->setMuted(false);

    filepath = QString("/home/grog/OSTs/ZeldaOOT/KokiriForest.mp3");
    player->setSource(QUrl::fromLocalFile(filepath));
    audioOutput->setVolume(0.5);
    player->play();
}