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
#include "Camera.h"
#include "Mode.h"



class MainOpenGLWidget: public QOpenGLWidget, QOpenGLFunctions {
Q_OBJECT
public:
    explicit MainOpenGLWidget(QWidget* parent = nullptr);
protected:
    void initializeGL() override;
    void resizeGL( int w, int h) override;
    void paintGL() override;
    ~MainOpenGLWidget() override;



protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    void keyPressEvent(QKeyEvent*) override;


private:
    float fov = 45.0f;
    bool leftButtonPressed = false;
    bool rightButtonPressed = false;
    QOpenGLShaderProgram program;
    QOpenGLVertexArrayObject VAO;
    QVector<QVector3D> vertices;
    VirtualBall vBall;
    MainWindow *parent;
    QStatusBar *statusBar;
    // shader uniform
    int uModel{};
    int uView{};
    int uProjection{};
    int uNormal{};

    // matrix
    QMatrix4x4 mProjection;
    QMatrix4x4 mModel;
    QMatrix4x4 mView;
    QMatrix4x4 mNormal;

    // camera
    Camera camera;

    // mode
    Mode *mode;
    DrawMode *drawMode;


    // draw array
    QVector<QVector3D> bezierCurve;
    QVector<QVector3D> nCurve;
    QVector<QVector3D> bSpline;
};


#endif //BEZIER_MAINOPENGLWIDGET_H
