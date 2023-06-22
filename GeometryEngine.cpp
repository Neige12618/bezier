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
    // 给我一个绘制nurbs曲线的算法
    buffer = *controlPoints;
    qDebug() << buffer;

    int n = buffer.length();
    QVector<QVector3D> result;
    int i = 0;
    double t = 0.0;
    while (i < n) {
        t += 1.0 / precision;
        result.push_back(deBoor(buffer, t));
        i++;
    }

    vbNCurve.bind();

    int count = (int) (result.length() * sizeof(QVector3D));
    vbNCurve.allocate(result.constData(), count);

    vbNCurve.release();


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

// 一维deCasteljau算法实现
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

// 二维deCasteljau算法实现(借助一维)
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


// deBoor算法实现
QVector3D GeometryEngine::deBoor(QVector<QVector3D> &controlPoints, double t) {
    int n = controlPoints.length() - 1;
    int p = degree;
    int k = 0;

    while (k < n - p && t >= controlPoints[k + 1].x()) {
        k += 1;
    }

    QVector<QVector3D> d;
    for (int i = 0; i <= p; i++) {
        d.push_back(controlPoints[i + k - p]);
    }

    for (int i = 1; i <= p; i++) {
        for (int j = p; j >= i; j--) {
            double w = (t - d[j].x()) / (d[j + i - 1].x() - d[j].x());
            d[j] = (1 - w) * d[j - 1] + w * d[j];
        }
    }

    return d[p];
} 

// 给我写一个deboor算法的三维版本！！！
QVector3D GeometryEngine::deBoor(QVector<QVector3D> &controlPoints, double u, double v) {
    int n = controlPoints.length() - 1;
    int k = degree;
    int r = 0;

    while (r < n - k && u >= controlPoints[r + 1].x()) {
        r++;
    }

    QVector<QVector3D> d;
    for (int i = 0; i <= k; i++) {
        d.push_back(controlPoints[r - i]);
    }

    for (int i = 1; i <= k; i++) {
        for (int j = k; j >= i; j--) {
            double w = (u - d[j].x()) / (d[j + i - 1].x() - d[j].x());
            d[j] = (1 - w) * d[j - 1] + w * d[j];
        }
    }

    return d[k];
}
