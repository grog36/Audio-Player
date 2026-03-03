#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QtMultimedia>
#include <string>
#include <vector>
#include <queue>

class QPushButton;
class QLabel;

class GameWindow : public QWidget {
    Q_OBJECT;

    public:
        GameWindow(QWidget* parent = 0);
        ~GameWindow();

    private:
        //Current Working Directory
        QString projectPath = QDir::currentPath().remove("/build");
        //Backend Song List
        std::vector<std::string> songList;
        std::queue<std::string> songQueue;
        //Media Control Buttons
        QPushButton* playPauseButton;
        QPushButton* nextButton;

        //Media Player Stuff
        QMediaPlayer* player;
        QAudioOutput* audioOutput;
        QString filepath;

        //Displayed Game Picture
        QLabel* gamePicture;

        //Helper Methods
        void readSongs();
        void shuffleQueue();
        int getRandomNumber(int max);
        std::string getNextSong();
        void getGame();

    private slots:
        void turnOnMusic();
        void nextTrack();
};


#endif


