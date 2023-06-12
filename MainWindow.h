//
// Created by Saatus on 2023/6/4.
//

#ifndef BEZIER_MAINWINDOW_H
#define BEZIER_MAINWINDOW_H
#include <QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QLabel>
#include <QOpenGLWidget>
#include "Mode.h"
#include "MainOpenGLWidget.h"


class MainWindow: public QMainWindow {
public:
    MainWindow();

private:
    MainOpenGLWidget *openglWidget;

    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QLabel *drawMode;
    QLabel *pos;
    QLabel *mode;

private:
    void initMenuBar();
    void initStatusBar();

protected:
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;

private slots:

    void modeBezierCurve();
    void modeNCurve();
    void modeBSpline();
    void modeBezierSurface();
    void modeNSurface();
    void modeBSplineSurface();
    void modeEdit();
    void modeView();

};


#endif //BEZIER_MAINWINDOW_H
