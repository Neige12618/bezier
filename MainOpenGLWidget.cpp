//
// Created by Saatus on 2023/6/4.
//

#include "MainOpenGLWidget.h"


GLfloat vertices[] = {
        -0.5f,  0.5f, 0.0f, // 左上
        0.5f,  0.5f,  0.0f, // 右上
        0.5f, -0.5f, 0.0f, // 右下
        -0.5f, -0.5f, 0.0f,// 左下
        1.0f, 1.0f, 0.0f,
};

float rotate = 0.0f;

MainOpenGLWidget::MainOpenGLWidget(QWidget *parent): QOpenGLWidget(parent) {
    auto *p = dynamic_cast<MainWindow*> (parent);
    statusBar = p->statusBar;
    statusBar->showMessage("opengl");
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_time_out())) ;
    timer.start(10);
}

void MainOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();


    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":shader/vertex.vert");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":shader/fragment.frag");
    program.addShaderFromSourceFile(QOpenGLShader::Geometry, ":shader/bezier.geom");


    program.link();
    program.bind();

    // create VBO
    QOpenGLBuffer VBO(QOpenGLBuffer::VertexBuffer);
    VBO.create();
    VBO.bind();

    VAO.create();
    VAO.bind();

    VBO.allocate(vertices, sizeof(vertices));

    program.setAttributeBuffer(0, GL_FLOAT, 0, 3);
    program.enableAttributeArray(0);

    program.release();
    VAO.release();
}

void MainOpenGLWidget::resizeGL(int w, int h) {
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void MainOpenGLWidget::paintGL() {
    QMatrix4x4 matrix;
    matrix.rotate(rotate, 0.0f, 1.0f, 0.0f);
    rotate = rotate > 360.0f ? 0.0f: rotate + 100.0f;

//    program.setUniformValue("rotationM", matrix);
    glPointSize(6);
    program.bind();
    VAO.bind();
    glDrawArrays(GL_LINES_ADJACENCY, 0, 4);
}

MainOpenGLWidget::~MainOpenGLWidget() {
    VAO.destroy();
}

void MainOpenGLWidget::on_time_out() {
//    update();
}
