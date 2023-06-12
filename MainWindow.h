//
// Created by Saatus on 2023/6/4.
//

#ifndef BEZIER_MAINWINDOW_H
#define BEZIER_MAINWINDOW_H
#include <QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QLabel>
#include "Mode.h"

class MainWindow: public QMainWindow {
public:
    MainWindow();

    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QLabel *modeBar;

    Mode mode;
    DrawMode drawMode;

private slots:
    void BezierCurve();

protected:
    void mouseMoveEvent(QMouseEvent* e) override;
};


#endif //BEZIER_MAINWINDOW_H
