//
// Created by Saatus on 2023/6/4.
//

#include "MainOpenGLWidget.h"
#include <QMouseEvent>


MainOpenGLWidget::MainOpenGLWidget(QWidget *parent): QOpenGLWidget(parent) {
    auto *p = dynamic_cast<MainWindow*> (parent);
    statusBar = p->statusBar;
    statusBar->showMessage("opengl");
    connect(&timer, SIGNAL(timeout()), this, SLOT(time_out())) ;
    timer.start(10);
    setFocus();
}

void MainOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":shader/vertex.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":shader/fragment.frag");

    program.link();
    program.bind();
    u_model = program.uniformLocation("model");
    u_view = program.uniformLocation("view");
    u_projection = program.uniformLocation("projection");
    u_normal = program.uniformLocation("normal");

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
    vBall.setBounds((float)w, (float)h);
//    glViewport(0, 0, w, h);


    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void MainOpenGLWidget::paintGL() {
    m_model.setToIdentity();
    m_view.setToIdentity();
    m_projection.setToIdentity();
    m_normal.setToIdentity();


    m_model = vBall.RotateMatrix();
    m_view.lookAt(cameraPos, QVector3D(0.0f, 0.0f, 0.0f),QVector3D(0.0f, 1.0f, 0.0f));
    m_projection.perspective(fov, (float)width() / (float)(height()), 0.1f, 1000.0f);

    program.bind();
    program.setUniformValue(u_model, m_model);
    program.setUniformValue(u_view, m_view);
    program.setUniformValue(u_projection, m_projection);
    program.setUniformValue(u_normal, m_normal);

    glDrawArrays(GL_LINES, 0, 4);
}

MainOpenGLWidget::~MainOpenGLWidget() {
    VAO.destroy();
}

void MainOpenGLWidget::time_out() {
//    update();
}


void MainOpenGLWidget::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        vBall.click((float)e->pos().x(), (float)e->pos().y());
        leftButtonPressed = true;
    } else if (e->button() == Qt::RightButton) {

    }
}


void MainOpenGLWidget::mouseMoveEvent(QMouseEvent* e) {
    if (leftButtonPressed) {
        vBall.dragTo(e->pos().x(), e->pos().y());
        update();
    }
}


void MainOpenGLWidget::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        vBall.push();
        leftButtonPressed = false;
    }
}


void MainOpenGLWidget::wheelEvent(QWheelEvent* e) {
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

    update();
}

void MainOpenGLWidget::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_W: {
            cameraPos.setY(cameraPos.y() + 1);
            break;
        }
        case Qt::Key_A: {
            cameraPos.setX(cameraPos.x() + 1);
            break;
        }
        case Qt::Key_S: {
            cameraPos.setY(cameraPos.y() - 1);
            break;
        }
        case Qt::Key_D: {
            cameraPos.setX(cameraPos.x() - 1);
            break;
        }
    }
    update();
}

