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
    GeometryEngine();
    void init();
    void draw(
        QOpenGLShaderProgram& _program,
        DrawMode drawMode,
        const QVector<QVector3D>& _controlPoints,
        Mode mode
    );
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
    QVector<QVector<QVector3D>> *controlPointsSurface = nullptr;
    QOpenGLShaderProgram *program = nullptr;
    int precision = 100;
    bool isSurface = false;

    QOpenGLBuffer* currentVBO = nullptr;
private:
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
