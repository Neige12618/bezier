//
// Created by Saatus on 2023/6/5.
//

#ifndef BEZIER_VIRTUALBALL_H
#define BEZIER_VIRTUALBALL_H
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>


class VirtualBall {
public:
    explicit VirtualBall(float width = 0, float height = 0);
    ~VirtualBall() = default;
    void setBounds(float new_width, float new_height);
    QMatrix4x4 RotateMatrix();
    QVector3D mapToSphere(float x, float y) const;
    void click(float x,float y);
    void dragTo(float x, float y);
    void push();
    static QMatrix4x4 quaternionToMat(QQuaternion& q);

private:
    QVector3D last_pos{}; //点击屏幕时，映射到球面的坐标
    QVector3D new_pos{};  //拖动后释放时，最后点映射到球面的坐标
    float _width{};
    float _height{};
    QQuaternion currentRotate{};  //当前旋转 四元数
    QQuaternion LastRotate{};  //前一个旋转 四元数
};


#endif //BEZIER_VIRTUALBALL_H
