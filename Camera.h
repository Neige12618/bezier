//
// Created by Saatus on 2023/6/10.
//

#ifndef BEZIER_CAMERA_H
#define BEZIER_CAMERA_H
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>


class Camera {
public:
    Camera();
    Camera(const QVector3D& Pos, const QVector3D& Target, QVector3D& Up);
    bool OnKeyBoard(int key);
    [[nodiscard]]
    const QVector3D& GetPos() const;
    [[nodiscard]]
    const QVector3D& GetTarget() const;
    [[nodiscard]]
    const QVector3D& GetUp() const;
    [[nodiscard]]
    const QVector3D& GetCenter();
    void click(const QPoint &pos);
    void dragTo(const QPoint &pos);

private:
    static void Rotate(QVector3D& src, float angle, const QVector3D& axe);

private:
    QVector3D m_pos;
    QVector3D m_target;
    QVector3D m_up;
    QVector3D m_center;
    QPoint last_pos;
    QPoint new_pos;
    float m_angleH = 0.0f;
    float m_angleV = 0.0f;

    float speed = 1.0f;
};


#endif //BEZIER_CAMERA_H
