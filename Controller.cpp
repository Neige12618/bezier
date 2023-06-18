//
// Created by Saatus on 2023/6/5.
//

#include "Controller.h"



void Controller::move(QPoint pos) {
    QVector2D diff =QVector2D(pos) - mousePressPosition;

    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0f).normalized();

    float acc = diff.length() / 10.0f;

    rotationAxis = (rotationAxis * angle + n * acc).normalized();

    angle = acc;

    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angle) * rotation;
}

void Controller::press(QPoint pos) {
    mousePressPosition = QVector2D(pos);
}


void Controller::release() {
    angle = 0;
}


const QQuaternion &Controller::getRotation() {
    return rotation;
}

void Controller::setRotateAxis(const QVector3D& r) {
    rotationAxis = r;
}