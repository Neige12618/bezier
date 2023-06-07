//
// Created by Saatus on 2023/6/4.
//

#ifndef BEZIER_MAINWINDOW_H
#define BEZIER_MAINWINDOW_H
#include <QMainWindow>
#include <QStatusBar>


class MainWindow: public QMainWindow {
public:
    MainWindow();

    QStatusBar *statusBar;
};


#endif //BEZIER_MAINWINDOW_H
