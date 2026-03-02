#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QtMultimedia>

class QPushButton;
class QLabel;

class GameWindow : public QWidget {
    Q_OBJECT;

    public:
        GameWindow(QWidget* parent = 0);
        ~GameWindow();

    private:
        QPushButton* startButton;
        QMediaPlayer* player;
        QAudioOutput* audioOutput;
        QString filepath;
        QLabel* gamePicture;

    private slots:
        void turnOnMusic();
};


#endif


