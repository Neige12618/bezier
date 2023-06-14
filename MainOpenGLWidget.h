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
#include <QStatusBar>
#include "VirtualBall.h"
#include "Camera.h"
#include "Mode.h"
#include "GeometryEngine.h"



class MainOpenGLWidget: public QOpenGLWidget, QOpenGLFunctions {
    Q_OBJECT
public:
    explicit MainOpenGLWidget(QWidget* parent = nullptr);
    ~MainOpenGLWidget() override;


protected:
    void initializeGL() override;
    void resizeGL( int w, int h) override;
    void paintGL() override;

private:
    void unProject(QVector3D &pos) const;


protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;


private:
    float fov = 45.0f;
    int width = 0;
    int height = 0;
    bool leftButtonPressed = false;
    bool rightButtonPressed = false;
    QOpenGLShaderProgram program;
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
    VirtualBall vBall;

    // mode
    Mode mode = Mode::EditMode;
    DrawMode drawMode = DrawMode::Bezier;


    // control points
    QVector<QVector3D> controlPoints;
    QVector<QVector<QVector3D>> controlPointsSurface;

    GeometryEngine *engine;

public slots:
    void clear();

public:
    void modeBezierCurve();
    void modeNCurve();
    void modeBSpline();
    void modeBezierSurface();
    void modeNSurface();
    void modeBSplineSurface();
    void modeEdit();
    void modeView();
};


#endif //BEZIER_MAINOPENGLWIDGET_H
