#include "Camera.h"

Camera::Camera() {
    m_pos = QVector3D(0.0f, 0.0f, 10.0f);
    m_target = QVector3D(0.0f, 0.0f, -1.0f);
    m_up = QVector3D(0.0f, 1.0f, 0.0f);
}

Camera::Camera(const QVector3D &Pos, const QVector3D &Target, QVector3D &Up):
    m_pos(Pos), m_target(Target), m_up(Up){
}


bool Camera::OnKeyBoard(int key) {
    bool ret = true;
    switch (key) {
        case Qt::Key_W: {
            m_pos += m_target * speed ;
            break;
        }
        case Qt::Key_A: {
            QVector3D left = QVector3D::crossProduct(m_up, m_target);
            left.normalize();
            m_pos += left * speed;
            break;
        }
        case Qt::Key_S: {
            m_pos -= m_target * speed;
            break;
        }
        case Qt::Key_D: {
            QVector3D right = QVector3D::crossProduct(m_target, m_up);
            right.normalize();
            m_pos += right * speed;
            break;
        }
        default: {
            ret = false;
        }
    }
    return ret;
}

const QVector3D& Camera::GetPos() const {
    return m_pos;
}


const QVector3D& Camera::GetTarget() const {
    return m_target;
}


const QVector3D& Camera::GetUp() const {
    return m_up;
}

const QVector3D& Camera::GetCenter() {
    m_center = m_pos + m_target;
    return m_center;
}


void Camera::click(const QPoint &pos) {
    last_pos = pos;
}

void Camera::dragTo(const QPoint &pos) {
    new_pos = pos;

    const QVector3D vAxis(0.0f, 1.0f, 0.0f);

    const int deltaX = new_pos.x() - last_pos.x();
    const int deltaY = new_pos.y() - last_pos.y();

    m_angleH += (float)deltaX / 20.0f;
    m_angleV += (float)deltaY / 20.0f;


    QVector3D view = QVector3D(0.0f, 0.0f, -1.0f);
    Rotate(view, m_angleH, vAxis);
    view.normalize();

    QVector3D hAxis = QVector3D::crossProduct(vAxis, view);
    hAxis.normalize();
    Rotate(view, -m_angleV, hAxis);
    view.normalize();

    m_target = view;
    m_target.normalize();

    m_up = QVector3D::crossProduct(m_target, hAxis);
    m_up.normalize();
}

void Camera::Rotate(QVector3D &src, float angle, const QVector3D &axe) {
    QQuaternion rotationQ = QQuaternion::fromAxisAndAngle(axe, angle);
    auto ans = rotationQ.rotatedVector(src);

    src = qMove(ans);
}


