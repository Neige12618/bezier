//
// Created by Saatus on 2023/6/4.
//

#include "MainOpenGLWidget.h"
#include <QMouseEvent>



MainOpenGLWidget::MainOpenGLWidget(QWidget *parent): QOpenGLWidget(parent) {
    this->parent = dynamic_cast<MainWindow*> (parent);
    setFocus();
    mode = &this->parent->mode;
    drawMode = &this->parent->drawMode;
    statusBar = this->parent->statusBar;
//    statusBar->showMessage("opengl");
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

    vertices.push_back({-0.5f,  0.5f, 0.0f}); // 左上
    vertices.push_back({0.5f,  0.5f,  0.0f}); // 右上
    vertices.push_back({ 0.5f, -0.5f, 0.0f}); // 右下
    vertices.push_back({-0.5f, -0.5f, 0.0f});// 左下
    vertices.push_back({1.0f, 1.0f, 0.0f});

    program.setAttributeArray(0, vertices.constData());
    program.enableAttributeArray(0);

    program.release();
}

void MainOpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);


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
    mProjection.perspective(fov, (float)width() / (float)(height()), 0.1f, 1000.0f);

    program.bind();
    program.setUniformValue(uModel, mModel);
    program.setUniformValue(uView, mView);
    program.setUniformValue(uProjection, mProjection);
    program.setUniformValue(uNormal, mNormal);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}


MainOpenGLWidget::~MainOpenGLWidget() {
    VAO.destroy();
}


void MainOpenGLWidget::mousePressEvent(QMouseEvent* e) {
    if (*mode == Mode::ViewMode) {
        if (e->button() == Qt::LeftButton) {
            camera.press(e->pos());
            leftButtonPressed = true;
        } else if (e->button() == Qt::RightButton) {
            vBall.press(e->pos());
            rightButtonPressed = true;
        }
    }
}


void MainOpenGLWidget::mouseMoveEvent(QMouseEvent* e) {
    QOpenGLWidget::mouseMoveEvent(e);
    if (*mode == Mode::ViewMode) {
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
    if (*mode == Mode::ViewMode) {
        if (e->button() == Qt::LeftButton) {
            leftButtonPressed = false;
        } else if (e->button() == Qt::RightButton) {
            rightButtonPressed = false;
            vBall.release();
        }
    }

}


void MainOpenGLWidget::wheelEvent(QWheelEvent* e) {
    if (*mode == Mode::ViewMode) {
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
    if (*mode == Mode::ViewMode) {
        camera.OnKeyBoard(e->key());
    }
    update();
}

