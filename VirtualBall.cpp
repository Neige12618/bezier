//
// Created by Saatus on 2023/6/5.
//

#include "VirtualBall.h"


VirtualBall::VirtualBall(float width , float height ): _height(height), _width(width) {
    LastRotate = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 0),0.0f);
    currentRotate = QQuaternion::fromAxisAndAngle(QVector3D(0, 0, 0), 0.0f);
}


void VirtualBall::setBounds(float new_height, float new_width) {
    _width = new_width;
    _height = new_height;
}


QMatrix4x4 VirtualBall::RotateMatrix() {
    return quaternionToMat(currentRotate);
}


QVector3D VirtualBall::mapToSphere(int x, int y) const {
    //首先将点[0,width],[0,height]映射到[-1,1],[1,-1]上
    float xx = (float)x * 2.0f / _width - 1;
    float yy = (float)y * (-2.0f) / _height + 1;
    float d = xx * xx + yy * yy;

    if (d <= 1) {
        return {xx, yy, qSqrt(1 - d)};
    } else {
        return {xx / qSqrt(d), yy / qSqrt(d), 0};
    }
}


void VirtualBall::click(const QPoint &pos) {
    last_pos = mapToSphere(pos.x(), pos.y());
}


void VirtualBall::dragTo(const QPoint &pos) {
    new_pos = mapToSphere(pos.x(), pos.y());

    QVector3D axis = QVector3D::crossProduct(last_pos, new_pos); //叉积为旋转轴
    axis.normalize();
    float angle = qAsin((QVector3D::crossProduct(last_pos, new_pos)).length()); //旋转角度
    angle = (float) (angle / M_PI * 180);
    //Quaternion参数中的角度不是弧度
    QQuaternion temp = QQuaternion::fromAxisAndAngle( axis, 2 * angle); //生成四元数，其中为了旋转360度，角度为2倍
    currentRotate = temp*LastRotate; //保存到currentRotate中
}


void VirtualBall::push() {
    LastRotate = currentRotate; //一次拖动完成，将currentRotate保存
}


QMatrix4x4 VirtualBall::quaternionToMat(QQuaternion& q) {
    //based on algorithm on wikipedia
    // http://en.wikipedia.org/wiki/Rotation_matrix#Quaternion
    float w = q.scalar();
    float x = q.x();
    float y = q.y();
    float z = q.z();

    float n = q.lengthSquared();
    float s = n == 0 ? 0 : 2 / n;
    float wx = s * w * x, wy = s * w * y, wz = s * w * z;
    float xx = s * x * x, xy = s * x * y, xz = s * x * z;
    float yy = s * y * y, yz = s * y * z, zz = s * z * z;

    QMatrix4x4 result = QMatrix4x4({1 - (yy + zz),         xy + wz ,         xz - wy ,0,
                                    xy - wz ,    1 - (xx + zz),         yz + wx ,0,
                                    xz + wy ,         yz - wx ,    1 - (xx + yy),0,
                                    0 ,               0 ,               0 ,1 });

    result.optimize();
    return result;
}