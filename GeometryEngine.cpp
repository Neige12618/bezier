//
// Created by Saatus on 2023/6/12.
//

#include "GeometryEngine.h"
#include <QRandomGenerator>

GeometryEngine::GeometryEngine(
    bool &_isSurface,
    QVector<QVector3D> &_controlPoints,
    QVector<QVector<QVector3D>> &_controlPointsSurface,
    QOpenGLShaderProgram &_program
){
    isSurface = &_isSurface;
    controlPoints = &_controlPoints;
    controlPointsSurface = &_controlPointsSurface;
    program = &_program;
}



void GeometryEngine::init() {
    initializeOpenGLFunctions();

    // curve
    vbBezierCurve.create();
    vbNCurve.create();
    vbBSpline.create();

    // surface
    vbBezierSurface.create();
    vbNSurface.create();
    vbBSplineSurface.create();

    currentVBO = &vbBezierCurve;
}

void GeometryEngine::draw() {
    drawControlPoints();
    drawCurrent();
}

void GeometryEngine::drawBezierCurve() {
    currentVBO = &vbBezierCurve;
    QVector<QVector3D> buffer;
    double t = 0.0;
    for (int i = 0; i < precision; i++) {
        t += 1.0 / precision;

        buffer.push_back(deCasteljau(*controlPoints, (float)t));
    }

    vbBezierCurve.bind();

    int count = (int) (buffer.length() * sizeof(QVector3D));
    vbBezierCurve.allocate(buffer.constData(), count);

    vbBezierCurve.release();
}


void GeometryEngine::drawNCurve() {
    currentVBO = &vbNCurve;

    QVector<QVector3D> buffer;
    buffer = *controlPoints;
    qDebug() << buffer;

}


void GeometryEngine::drawBSpline() {
    currentVBO = &vbBSpline;

    QVector<QVector3D> buffer;
    buffer = *controlPoints;
    qDebug() << buffer;

}




void GeometryEngine::drawBezierSurface() {
    currentVBO = &vbBezierSurface;

    QVector<QVector3D> buffer;

    QVector<QVector<QVector3D>> out;
    // 获得曲面上的所有点
    float u, v;
    for (int i = 0; i < precision; i++) {
        u = (float)i / (float)precision;
        QVector<QVector3D> temp;
        for (int j = 0; j < precision; j++) {
            v = float(j) / (float) precision;
            temp.push_back(deCasteljau(u, v));
        }
        out.emplace_back(qMove(temp));
    }

    auto iLength = out.length();
    auto jLength = out[0].length();

    // 构造三角形
    for (int i = 0; i < iLength - 1; i++) {
        for (int j = 0; j < jLength - 1; j++) {
            buffer.push_back(out[i][j]);
            buffer.push_back(out[i+1][j]);
            buffer.push_back(out[i+1][j+1]);


            buffer.push_back(out[i][j]);
            buffer.push_back(out[i+1][j+1]);
            buffer.push_back(out[i][j+1]);
        }
    }



    vbBezierSurface.bind();

    int count = (int) (buffer.length() * sizeof(QVector3D));
    vbBezierSurface.allocate(buffer.constData(), count);

    vbBezierCurve.release();
}


void GeometryEngine::drawNSurface() {
    currentVBO = &vbNSurface;

    QVector<QVector3D> buffer;
    buffer = *controlPoints;
    qDebug() << buffer;

}


void GeometryEngine::drawBSplineSurface() {
    currentVBO = &vbBSplineSurface;

    QVector<QVector3D> buffer;
    buffer = *controlPoints;
    qDebug() << buffer;

}



void GeometryEngine::drawCurrent() {
    program->bind();
    currentVBO->bind();

    program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    program->enableAttributeArray(0);
    int count = (int)(currentVBO->size() / sizeof(QVector3D));
    if (*isSurface) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, count);
    } else {
        glDrawArrays(GL_LINE_STRIP, 0, count);
    }


    currentVBO->release();
    program->release();
}


void GeometryEngine::drawControlPoints() {
    program->bind();
    if (*isSurface) {

        for (auto &i: *controlPointsSurface) {
            program->setAttributeArray(0, i.constData());
            program->enableAttributeArray(0);


            glPointSize(10.0f);
            glDrawArrays(GL_LINE_STRIP, 0, (int) i.length());
            glDrawArrays(GL_POINTS, 0, (int) i.length());
        }


    } else {
        program->setAttributeArray(0, controlPoints->constData());
        program->enableAttributeArray(0);


        glPointSize(10.0f);
        glDrawArrays(GL_LINE_STRIP, 0, (int) controlPoints->length());
        glDrawArrays(GL_POINTS, 0, (int) controlPoints->length());
    }
    program->release();
}


QVector3D GeometryEngine::deCasteljau(const QVector<QVector3D>& control, float t) {
    auto n = control.length();
    if (n == 0) {
        return {};
    }

    auto point = control;

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < n - i; j++) {
            point[j] = (1 - t) * point[j] + t * point[j+1];
        }
    }

    return point[0];
}

QVector3D GeometryEngine::deCasteljau(float u, float v) {
    auto n = controlPointsSurface->length();
    if (n == 0) {
        return {};
    }
    QVector<QVector3D> q(n);

    for (int k = 0; k < n; k ++){
        q[k] = deCasteljau(controlPointsSurface->at(k), v);
    }


    return deCasteljau(q, u);
}


