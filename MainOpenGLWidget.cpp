//
// Created by Saatus on 2023/6/4.
//

#include "MainOpenGLWidget.h"
#include <QMouseEvent>
#include <QRandomGenerator>


MainOpenGLWidget::MainOpenGLWidget(QWidget *parent): QOpenGLWidget(parent) {
    engine = new GeometryEngine(isSurface,
                                controlPoints,
                                controlPointsSurface,
                                program);
    setFocus();
}

void MainOpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    engine->init();

    // 添加shader
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


    QRandomGenerator gen;
    gen.seed(4264);

    // 随机生成一个曲面
    controlPointsSurface.resize(controlRow);
    for (int i = 0; i < controlRow; i++) {
        controlPointsSurface[i].resize(degree);
        for (int j = 0; j < degree; j++) {
            float t = (float)(gen.generate() % 10000) / 5000.0f - 1;
            controlPointsSurface[i][j] = {(float)i, (float)j, t};
        }
    }


    program.release();
    modeBezierCurve();
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

    mModel.rotate(controller.getRotation());
    mView.lookAt(camera.GetPos(), camera.GetCenter(), camera.GetUp());
    mProjection.perspective(fov, (float)width / (float)height, 0.1f, 1000.0f);

    program.bind();
    program.setUniformValue(uModel, mModel);
    program.setUniformValue(uView, mView);
    program.setUniformValue(uProjection, mProjection);
    program.setUniformValue(uNormal, mNormal);
    program.release();


    engine->draw();
}


MainOpenGLWidget::~MainOpenGLWidget() {
    makeCurrent();
    program.destroyed();
    delete engine;
    doneCurrent();
}


void MainOpenGLWidget::mousePressEvent(QMouseEvent* e) {
    QOpenGLWidget::mousePressEvent(e);

    if (mode == Mode::ViewMode) {
        if (e->button() == Qt::LeftButton) {
            // 左键按下, 控制镜头移动
            camera.press(e->pos());
            leftButtonPressed = true;
        } else if (e->button() == Qt::RightButton) {
            // 右键按下, 控制模型旋转
            controller.press(e->pos());
            rightButtonPressed = true;
        }
    } else if (mode == Mode::EditMode) {
        if (e->button() == Qt::LeftButton) {
            // 左键按下, 添加控制点
            float x = (float) e->pos().x() ;
            float y = (float)height - (float) e->pos().y() ;

            // 计算屏幕坐标到世界坐标的映射
            QVector3D v = {x, y, 0};
            unProject(v);

            changeControlPoint(qMove(v), true);

            leftButtonPressed = true;
        } else if (e->button() == Qt::RightButton) {
            changeControlPoint();
            rightButtonPressed = true;
        }
        update();
    }
}


void MainOpenGLWidget::mouseMoveEvent(QMouseEvent* e) {
    QOpenGLWidget::mouseMoveEvent(e);
    if (mode == Mode::ViewMode) {
        // qt 默认按下左键才会有move检测, 其实可删除判断
        if (leftButtonPressed) {
            // 左键按下过, 移动镜头
            camera.move(e->pos());
            camera.press(e->pos());
        }
        if (rightButtonPressed) {
            // 右键按下过, 转动模型
            controller.move(e->pos());
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
            controller.release();
        }
    }

}


void MainOpenGLWidget::wheelEvent(QWheelEvent* e) {
    if (mode == Mode::ViewMode) {
        float offset;
        // 判断正负
        offset = e->angleDelta().y() < 0 ? -1.0f : 1.0f;

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
        // 所有键盘响应交给摄像机处理
        camera.OnKeyBoard(e->key());
    }
    update();
}


void MainOpenGLWidget::modeBezierCurve() {
    drawMode = DrawMode::Bezier;
    isSurface = false;
    engine->drawBezierCurve();
    update();
}

void MainOpenGLWidget::modeNCurve() {
    drawMode = DrawMode::NURBS;
    isSurface = false;
    engine->drawNCurve();
    update();
}

void MainOpenGLWidget::modeBSpline() {
    drawMode = DrawMode::BSpline;
    isSurface = false;
    engine->drawBSpline();
    update();
}

void MainOpenGLWidget::modeBezierSurface() {
    drawMode = DrawMode::BezierSurface;
    isSurface = true;
    engine->drawBezierSurface();
    update();
}

void MainOpenGLWidget::modeNSurface() {
    drawMode = DrawMode::NURBSSurface;
    isSurface = true;
    engine->drawNSurface();
    update();
}

void MainOpenGLWidget::modeBSplineSurface() {
    drawMode = DrawMode::BSplineSurface;
    isSurface = true;
    engine->drawBSplineSurface();
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

// 用于将屏幕坐标转换为世界坐标
void MainOpenGLWidget::unProject(QVector3D &pos) const {
    QMatrix4x4 projection;
    projection.setToIdentity();
    projection.perspective(fov, (float)width / (float)height, 10.0f, 1000.0f);

    pos = pos.unproject( mView * mModel, projection, {0, 0, width, height});
    pos.setY(pos.y());

}

void MainOpenGLWidget::clear() {
    changeControlPoint({}, false, true);
    update();
}

void MainOpenGLWidget::changeControlPoint(QVector3D &&v, bool add, bool clear) {
    if (clear) {
        if (isSurface) {
            for (auto &i: controlPointsSurface) {
                i.clear();
            }
            controlPointsSurface.clear();
        } else {
            controlPoints.clear();
        }
    } else {
        if (add) {
            if (isSurface) {
                // 左键按下后的曲面控制点加入
                if (controlPointsSurface.empty() || controlPointsSurface.last().length() >= degree) {
                    controlPointsSurface.push_back({});
                }
                controlPointsSurface.last().push_back(v);
                } else {
                    controlPoints.push_back(v);
                }
        } else {
            if (isSurface) {
                // 右键按下后的曲面控制点删除
                if (!controlPointsSurface.empty()) {
                    controlPointsSurface.last().pop_back();

                    if (controlPointsSurface.last().empty()) {
                        if (!controlPointsSurface.empty()) {
                            controlPointsSurface.pop_back();
                        }
                    }
                }
            } else {
                // 右键按下, 如果控制点不为空, 删除控制点
                if (!controlPoints.empty()) {
                    controlPoints.pop_back();
                }
            }
        }
    }

    switch (drawMode) {
        case Bezier: {
            engine->drawBezierCurve();
            break;
        }
        case NURBS: {
            engine->drawNCurve();
            break;
        }
        case BSpline: {
            engine->drawBezierCurve();
            break;
        }
        case BezierSurface: {
            engine->drawBezierSurface();
            break;
        }
        case NURBSSurface: {
            engine->drawNSurface();
            break;
        }
        case BSplineSurface: {
            engine->drawBSplineSurface();
            break;
        }
        default: {
            break;
        }
    }
}

void MainOpenGLWidget::setDegree(int d) {
    degree = d;
}







