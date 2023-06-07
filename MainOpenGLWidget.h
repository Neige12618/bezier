//
// Created by Saatus on 2023/6/4.
//

#ifndef BEZIER_MAINOPENGLWIDGET_H
#define BEZIER_MAINOPENGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QTime>
#include <QTimer>
#include "MainWindow.h"



class MainOpenGLWidget: public QOpenGLWidget, QOpenGLFunctions {
Q_OBJECT
public:
    explicit MainOpenGLWidget(QWidget* parent = nullptr);
protected:
    void initializeGL() override;
    void resizeGL( int w, int h) override;
    void paintGL() override;
    ~MainOpenGLWidget() override;
private slots:
    void on_time_out();
private:
    QStatusBar *statusBar;
    QOpenGLShaderProgram program;
    QOpenGLVertexArrayObject VAO;
    QTimer timer;
};


#endif //BEZIER_MAINOPENGLWIDGET_H
