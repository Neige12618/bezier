//
// Created by Saatus on 2023/6/4.
//

#include "MainWindow.h"
#include <QMouseEvent>

MainWindow::MainWindow(): mode(Mode::ViewMode), drawMode(DrawMode::Bezier) {
    resize(800, 600);
    statusBar = new QStatusBar(this);
    modeBar = new QLabel("mode:");
    statusBar->addWidget(modeBar);
    setStatusBar(statusBar);
    menuBar = new QMenuBar(this);
    menuBar->addAction("bezier", this, &MainWindow::BezierCurve);

    setMenuBar(menuBar);
}


void MainWindow::BezierCurve() {
    drawMode = DrawMode::Bezier;
    modeBar->setText("mode: Bezier Curve");
//    statusBar->showMessage("Bezier Curve");
    qDebug() << statusBar->currentMessage();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e) {
    QString s = QString::number(e->pos().x()) + " " + QString::number(e->pos().y());
    modeBar->setText(s);

}



