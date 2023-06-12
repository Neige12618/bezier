//
// Created by Saatus on 2023/6/4.
//

#include "MainOpenGLWidget.h"
#include <QMouseEvent>



MainOpenGLWidget::MainOpenGLWidget(QWidget *parent): QOpenGLWidget(parent) {
    engine = new GeometryEngine;
    setFocus();
}

void MainOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":shader/vertex.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":shader/fragment.frag");

    program.link();
    program.bind();

    uModel = program.uniformLocation("model");
    uView = program.uniformLocation("view");
    uProjection = program.uniformLocation("projection");
    uNormal = program.uniformLocation("normal");

    controlPoints.push_back({-0.5f,  0.5f, 0.0f}); // 左上
    controlPoints.push_back({0.5f,  0.5f,  0.0f}); // 右上
    controlPoints.push_back({ 0.5f, -0.5f, 0.0f}); // 右下
    controlPoints.push_back({-0.5f, -0.5f, 0.0f});// 左下
//    controlPoints.push_back({1.0f, 1.0f, 0.0f});


    program.release();
}

void MainOpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    width = w;
    height = h;

    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void MainOpenGLWidget::paintGL() {
    mModel.setToIdentity();
    mView.setToIdentity();
    mProjection.setToIdentity();
    mNormal.setToIdentity();


    mModel.rotate(vBall.getRotation());
    mView.lookAt(camera.GetPos(), camera.GetCenter(), camera.GetUp());
    mProjection.perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);


    program.bind();
    program.setUniformValue(uModel, mModel);
    program.setUniformValue(uView, mView);
    program.setUniformValue(uProjection, mProjection);
    program.setUniformValue(uNormal, mNormal);

    engine->draw(program, drawMode, controlPoints);
}


MainOpenGLWidget::~MainOpenGLWidget() {
    makeCurrent();
    program.destroyed();
    doneCurrent();
}


void MainOpenGLWidget::mousePressEvent(QMouseEvent* e) {
    QOpenGLWidget::mousePressEvent(e);

    if (mode == Mode::ViewMode) {
        if (e->button() == Qt::LeftButton) {
            camera.press(e->pos());
            leftButtonPressed = true;
        } else if (e->button() == Qt::RightButton) {
            vBall.press(e->pos());
            rightButtonPressed = true;
        }
    } else if (mode == Mode::EditMode) {
        if (e->button() == Qt::LeftButton) {
            float x = (float) e->pos().x() ;
            float y = (float)height - (float) e->pos().y() ;

            QVector3D v = {x, y, 0};
            unProject(v);

            controlPoints.push_back(
                v
            );

            leftButtonPressed = true;
        } else if (e->button() == Qt::RightButton) {
            controlPoints.pop_back();
            rightButtonPressed = true;
        }
        update();
    }
}


void MainOpenGLWidget::mouseMoveEvent(QMouseEvent* e) {
    QOpenGLWidget::mouseMoveEvent(e);
    if (mode == Mode::ViewMode) {
        if (leftButtonPressed) {
            camera.move(e->pos());
            camera.press(e->pos());
        }
        if (rightButtonPressed) {
            vBall.move(e->pos());
        }
    }
    update();

}


void MainOpenGLWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (mode == Mode::ViewMode) {
        if (e->button() == Qt::LeftButton) {
            leftButtonPressed = false;
        } else if (e->button() == Qt::RightButton) {
            rightButtonPressed = false;
            vBall.release();
        }
    }

}


void MainOpenGLWidget::wheelEvent(QWheelEvent* e) {
    if (mode == Mode::ViewMode) {
        float offset;
        if (e->angleDelta().y() & 0x80000000) {
            offset = -1;
        } else {
            offset = 1;
        }

        if (fov >= 1.0f && fov <= 89.0f) {
            fov -= offset;
        }
        if (fov < 1.0f) {
            fov = 1.0f;
        } else if (fov > 89.0f) {
            fov = 89.0f;
        }
    }
    update();
}

void MainOpenGLWidget::keyPressEvent(QKeyEvent *e) {
    QOpenGLWidget::keyPressEvent(e);
    if (mode == Mode::ViewMode) {
        camera.OnKeyBoard(e->key());
    }
    update();
}


void MainOpenGLWidget::modeBezierCurve() {
    drawMode = DrawMode::Bezier;
    update();
}

void MainOpenGLWidget::modeNCurve() {
    drawMode = DrawMode::NURBS;
    update();
}

void MainOpenGLWidget::modeBSpline() {
    drawMode = DrawMode::BSpline;
    update();
}

void MainOpenGLWidget::modeBezierSurface() {
    drawMode = DrawMode::BezierSurface;
    update();
}

void MainOpenGLWidget::modeNSurface() {
    drawMode = DrawMode::NURBSSurface;
    update();
}

void MainOpenGLWidget::modeBSplineSurface() {
    drawMode = DrawMode::BSplineSurface;
    update();
}

void MainOpenGLWidget::modeEdit() {
    mode = Mode::EditMode;
    rightButtonPressed = false;
    leftButtonPressed =false;
    update();
}


void MainOpenGLWidget::modeView() {
    mode = Mode::ViewMode;
    rightButtonPressed = false;
    leftButtonPressed =false;
    update();
}

void MainOpenGLWidget::unProject(QVector3D &pos) const {
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(fov, (float)width / (float)height, 10.0f, 1000.0f);

    pos = pos.unproject( mView, projection, {0, 0, width, height});
    pos.setY(pos.y());

}







