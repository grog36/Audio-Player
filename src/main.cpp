#include <QtWidgets/QApplication>
#include "gamewindow.h"

int main(int argc, char* argv[]) {
    QApplication ap(argc, argv);
    GameWindow gw;
    gw.setStyleSheet("background-color: tan;");
    gw.show();
    return ap.exec();
}
