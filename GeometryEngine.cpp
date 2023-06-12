//
// Created by Saatus on 2023/6/12.
//

#include "GeometryEngine.h"


GeometryEngine::GeometryEngine() {
    init();
}

void GeometryEngine::init() {

}

void GeometryEngine::draw(
    QOpenGLShaderProgram& _program,
    DrawMode mode,
    const QVector<QVector3D>& _controlPoints
) {
    initializeOpenGLFunctions();

    this->controlPoints = &_controlPoints;
    this->program = &_program;
    switch (mode) {
        case Bezier: {
            drawBezierCurve();
            break;
        }
        case NURBS: {
            drawNCurve();
            break;
        };
        case BSpline: {
            drawBSpline();
            break;
        };
        case BezierSurface: {
            drawBezierSurface();
            break;
        };
        case NURBSSurface: {
            drawNSurface();
            break;
        };
        case BSplineSurface: {
            drawBSplineSurface();
            break;
        };
        default: {
            break;
        }
    }
}

void GeometryEngine::drawBezierCurve() {
    vBezierCurve.clear();

    vBezierCurve = *controlPoints;
    program->setAttributeArray(0, vBezierCurve.constData());
    program->enableAttributeArray(0);

    glPointSize(20.0f);
    glDrawArrays(GL_POINTS, 0, (int) vBezierCurve.length());

}


void GeometryEngine::drawNCurve() {
    vNCurve.clear();

    vNCurve = *controlPoints;
    program->setAttributeArray(0, vNCurve.constData());
    program->enableAttributeArray(0);
    glDrawArrays(GL_LINES, 0, (int) vNCurve.length());
}


void GeometryEngine::drawBSpline() {
    vBSpline.clear();
}


void GeometryEngine::drawBezierSurface() {
    vBezierSurface.clear();
}


void GeometryEngine::drawNSurface() {
    vNSurface.clear();
}


void GeometryEngine::drawBSplineSurface() {
    vBSplineSurface.clear();
}





