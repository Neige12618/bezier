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
#include "VirtualBall.h"



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
    void time_out();


protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    void keyPressEvent(QKeyEvent*) override;


private:
    float fov = 45.0f;
    bool leftButtonPressed = false;
    QStatusBar *statusBar;
    QOpenGLShaderProgram program;
    QOpenGLVertexArrayObject VAO;
    QVector<QVector3D> vertices;
    VirtualBall vBall;
    QTimer timer;
    // shader uniform
    int u_model{};
    int u_view{};
    int u_projection{};
    int u_normal{};

    // matrix
    QMatrix4x4 m_projection;
    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_normal;


    QVector3D cameraPos{0.0f, 0.0f, 15.0f};
};


#endif //BEZIER_MAINOPENGLWIDGET_H
