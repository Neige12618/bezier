//
// Created by Saatus on 2023/6/4.
//

#include "MainWindow.h"
#include <QMouseEvent>

MainWindow::MainWindow():
    openglWidget(new MainOpenGLWidget(this)),
    statusBar(new QStatusBar(this)),
    menuBar(new QMenuBar(this)),
    drawMode(new QLabel),
    pos(new QLabel),
    mode(new QLabel)
{
    initMenuBar();
    initStatusBar();

    setCentralWidget(openglWidget);
    resize(800, 600);
}


void MainWindow::initMenuBar() {

    menuBar->addAction("bezier  |", this, &MainWindow::modeBezierCurve);
    menuBar->addAction("NURBS  |", this, &MainWindow::modeNCurve);
    menuBar->addAction("BSpline  |", this, &MainWindow::modeBSpline);
    menuBar->addAction("bezier surface  |", this, &MainWindow::modeBezierSurface);
    menuBar->addAction("NURBS surface  |", this, &MainWindow::modeNSurface);
    menuBar->addAction("BSpline surface  |", this, &MainWindow::modeBSplineSurface);
    menuBar->addAction("view mode  |", this, &MainWindow::modeView);
    menuBar->addAction("edit mode  |", this, &MainWindow::modeEdit);


    setMenuBar(menuBar);
}


void MainWindow::initStatusBar() {
    statusBar->addWidget(mode);
    statusBar->addWidget(drawMode);
    statusBar->addWidget(pos);
    mode->setText("Edit Mode");
    drawMode->setText("bezier");

    setStatusBar(statusBar);
}


void MainWindow::modeBezierCurve() {
    openglWidget->modeBezierCurve();
    drawMode->setText("bezier");
}

void MainWindow::modeNCurve() {
    openglWidget->modeNCurve();
    drawMode->setText("NURBS");
}

void MainWindow::modeBSpline() {
    openglWidget->modeBSpline();
    drawMode->setText("BSpline");
}

void MainWindow::modeBezierSurface() {
    openglWidget->modeBezierSurface();
    drawMode->setText("bezier surface");
}

void MainWindow::modeNSurface() {
    openglWidget->modeNSurface();
    drawMode->setText("NURBS surface");
}

void MainWindow::modeBSplineSurface() {
    openglWidget->modeBSplineSurface();
    drawMode->setText("BSpline surface");
}

void MainWindow::modeEdit() {
    openglWidget->modeEdit();
    mode->setText("Edit Mode");
}

void MainWindow::modeView() {
    openglWidget->modeView();
    mode->setText("View Mode");
}

void MainWindow::mouseMoveEvent(QMouseEvent *e) {
    QWidget::mouseMoveEvent(e);

    pos->setText(
            "x: " +
            QString::number(e->pos().x()) +
            " y: " +
            QString::number(e->pos().y())
            );
}

void MainWindow::mousePressEvent(QMouseEvent *e) {
    QWidget::mousePressEvent(e);


    pos->setText(
            "x: " +
            QString::number(e->pos().x()) +
            " y: " +
            QString::number(e->pos().y())
    );
}
