//
// Created by Saatus on 2023/6/4.
//

#include "MainWindow.h"

MainWindow::MainWindow() {
    resize(800, 600);
    statusBar = new QStatusBar;
    setStatusBar(statusBar);
}