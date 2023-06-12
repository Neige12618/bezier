//
// Created by Saatus on 2023/6/12.
//

#ifndef BEZIER_GEOMETRYENGINE_H
#define BEZIER_GEOMETRYENGINE_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include "Mode.h"

class GeometryEngine: protected QOpenGLFunctions{
public:
    GeometryEngine();
    void init();
    void draw(
        QOpenGLShaderProgram& _program,
        DrawMode mode,
        const QVector<QVector3D>& _controlPoints
    );
    ~GeometryEngine() = default;

private:
    // curve
    QVector<QVector3D> vBezierCurve;
    QVector<QVector3D> vNCurve;
    QVector<QVector3D> vBSpline;

    // surface
    QVector<QVector3D> vBezierSurface;
    QVector<QVector3D> vNSurface;
    QVector<QVector3D> vBSplineSurface;

    const QVector<QVector3D> *controlPoints = nullptr;
    QOpenGLShaderProgram *program = nullptr;


private:
    void drawBezierCurve();
    void drawNCurve();
    void drawBSpline();
    void drawBezierSurface();
    void drawNSurface();
    void drawBSplineSurface();
};


#endif //BEZIER_GEOMETRYENGINE_H
