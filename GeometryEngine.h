//
// Created by Saatus on 2023/6/12.
//

#ifndef BEZIER_GEOMETRYENGINE_H
#define BEZIER_GEOMETRYENGINE_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "Mode.h"

class GeometryEngine: protected QOpenGLFunctions{
public:
    GeometryEngine(
        bool &_isSurface,
        QVector<QVector3D> &_controlPoints,
        QVector<QVector<QVector3D>> &_controlPointsSurface,
        QOpenGLShaderProgram &_program
    );
    void init();
    void draw();
    ~GeometryEngine() = default;

private:
    // curve
    QOpenGLBuffer vbBezierCurve;
    QOpenGLBuffer vbNCurve;
    QOpenGLBuffer vbBSpline;

    // surface
    QOpenGLBuffer vbBezierSurface;
    QOpenGLBuffer vbNSurface;
    QOpenGLBuffer vbBSplineSurface;


    const QVector<QVector3D> *controlPoints = nullptr;
    const QVector<QVector<QVector3D>> *controlPointsSurface = nullptr;
    QOpenGLShaderProgram *program = nullptr;
    int precision = 100;
    bool *isSurface = nullptr;


    QOpenGLBuffer* currentVBO = nullptr;
public:
    void drawBezierCurve();
    void drawNCurve();
    void drawBSpline();
    void drawBezierSurface();
    void drawNSurface();
    void drawBSplineSurface();
    void drawCurrent();
    void drawControlPoints();

    static QVector3D deCasteljau(const QVector<QVector3D>& controlPoints, float t);
    QVector3D deCasteljau(float u, float v);
};


#endif //BEZIER_GEOMETRYENGINE_H
