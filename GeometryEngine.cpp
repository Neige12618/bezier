//
// Created by Saatus on 2023/6/12.
//

#include "GeometryEngine.h"
#include <QRandomGenerator>

GeometryEngine::GeometryEngine() = default;



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

void GeometryEngine::draw(
    QOpenGLShaderProgram& _program,
    DrawMode drawMode,
    const QVector<QVector3D>& _controlPoints,
    Mode mode
) {


    this->controlPoints = &_controlPoints;
    this->program = &_program;

    if (controlPoints->empty()) {
        return ;
    }

    drawControlPoints();
    if (mode == Mode::ViewMode) {
        drawCurrent();
        return ;
    }

    switch (drawMode) {
        case Bezier: {
            isSurface = false;
            currentVBO = &vbBezierCurve;
            drawBezierCurve();
            break;
        }
        case NURBS: {
            isSurface = false;
            currentVBO = &vbNCurve;
            drawNCurve();
            break;
        }
        case BSpline: {
            isSurface = false;
            currentVBO = &vbBSpline;
            drawBSpline();
            break;
        }
        case BezierSurface: {
            isSurface = true;
            currentVBO = &vbBezierSurface;
            drawBezierSurface();
            break;
        }
        case NURBSSurface: {
            isSurface = true;
            currentVBO = &vbNSurface;
            drawNSurface();
            break;
        }
        case BSplineSurface: {
            isSurface = true;
            currentVBO = &vbBSplineSurface;
            drawBSplineSurface();
            break;
        }
        default: {
            break;
        }
    }

}

void GeometryEngine::drawBezierCurve() {
    QVector<QVector3D> buffer;
    double t = 0.0;
    for (int i = 0; i < precision; i++) {
        t += 1.0 / precision;

        buffer.push_back(deCasteljau(*controlPoints, (float)t));
    }

    program->bind();
    vbBezierCurve.bind();

    int count = (int) (buffer.length() * sizeof(QVector3D));
    vbBezierCurve.allocate(buffer.constData(), count);

    program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    program->enableAttributeArray(0);
    glDrawArrays(GL_LINE_STRIP, 0, (int) buffer.length());


    vbBezierCurve.release();
    program->release();
}


void GeometryEngine::drawNCurve() {
    QVector<QVector3D> buffer;


}


void GeometryEngine::drawBSpline() {
    QVector<QVector3D> buffer;

}




void GeometryEngine::drawBezierSurface() {
    const int NI = 8;
    const int NJ = 9;

    QVector<QVector3D> buffer;
    controlPointsSurface = new QVector<QVector<QVector3D>>(NI + 1, QVector<QVector3D>(NJ + 1));


    QRandomGenerator gen;
    gen.seed(4264);

    for (int i = 0; i <= NI; i++) {
        for (int j = 0; j <= NJ; j++) {
            float t = (float)(gen.generate() % 10000) / 5000.0f - 1;
            (*controlPointsSurface)[i][j] = {(float)i, (float)j, t};
        }
    }


    QVector<QVector<QVector3D>> out;
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

    program->bind();
    auto iLength = out.length();
    auto jLength = out[0].length();


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

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    program->enableAttributeArray(0);
    glDrawArrays(GL_TRIANGLES, 0, (int) buffer.length());


    vbBezierCurve.release();
    program->release();

}


void GeometryEngine::drawNSurface() {
    QVector<QVector3D> buffer;
}


void GeometryEngine::drawBSplineSurface() {
    QVector<QVector3D> buffer;
}



void GeometryEngine::drawCurrent() {
    program->bind();
    currentVBO->bind();

    program->setAttributeBuffer(0, GL_FLOAT, 0, 3);
    program->enableAttributeArray(0);
    int count = (int)(currentVBO->size() / sizeof(QVector3D));
    if (isSurface) {
        glDrawArrays(GL_TRIANGLES, 0, count);
    } else {
        glDrawArrays(GL_LINE_STRIP, 0, count);
    }


    currentVBO->release();
    program->release();
}


void GeometryEngine::drawControlPoints() {
    if (isSurface) {
        program->bind();
        for (auto &i: *controlPointsSurface) {
            program->setAttributeArray(0, i.constData());
            program->enableAttributeArray(0);


            glPointSize(10.0f);
            glDrawArrays(GL_LINE_LOOP, 0, (int) i.length());
            glDrawArrays(GL_POINTS, 0, (int) i.length());
        }

        program->release();
    } else {
        program->bind();
        program->setAttributeArray(0, controlPoints->constData());
        program->enableAttributeArray(0);


        glPointSize(10.0f);
        glDrawArrays(GL_LINE_STRIP, 0, (int) controlPoints->length());
        glDrawArrays(GL_POINTS, 0, (int) controlPoints->length());
        program->release();
    }

}


QVector3D GeometryEngine::deCasteljau(const QVector<QVector3D>& control, float t) {
    auto n = control.length();

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
    QVector<QVector3D> q(n);

    for (int k = 0; k < n; k ++){
        q[k] = deCasteljau(controlPointsSurface->at(k), v);
    }


    return deCasteljau(q, u);
}


