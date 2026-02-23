#include <QtWidgets>
#include "gamewindow.h"

//Constructor
GameWindow::GameWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle(tr("Grog's Audio Player"));
}

//Destructor
GameWindow::~GameWindow() {
    //Do more stuff
}