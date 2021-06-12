#include "CG_ViewportCamera.h"

CGCamera* CGCamera::activeCamera_ = nullptr;


CGCamera::CGCamera()
{
    position_ = QVector3D(0.0f, 0.0f, 0.0f);

    yaw_ = 0.0f;
    roll_ = 0.0f;
    pitch_ = 0.0f;

    projectionView_ = CGE::PERSPECTIVE;

    verticalAngle_ = 45.0f;
    aspectRatio_ = 1.77f;

    nearPlane_ = 0.01f;
    farPlane_ = 100.0f;

    orthoWindowSize_left_ = 0.0f;
    orthoWindowSize_bottom_ = 0.0f;
    orthoWindowSize_right_ = 1920.0f;
    orthoWindowSize_top_ = 1080.0f;


    projectionMatrix_ = QMatrix4x4();
    projectionMatrix_.setToIdentity();

    viewMatrix_ = QMatrix4x4();
    viewMatrix_.setToIdentity();

    direction_ = QVector3D(0.0f, 0.0f, -1.0f);
    right_ = QVector3D(1.0f, 0.0f, 0.0f);
    up_ = QVector3D(0.0f, 1.0f, 0.0f);
}

CGCamera::CGCamera(const CGCamera& other)
{
    position_ = other.position_;

    yaw_ = other.yaw_;
    roll_ = other.roll_;
    pitch_ = other.pitch_;

    projectionView_ = other.projectionView_;

    verticalAngle_ = other.verticalAngle_;
    aspectRatio_ = other.aspectRatio_;

    nearPlane_ = other.nearPlane_;
    farPlane_ = other.farPlane_;

    orthoWindowSize_left_ = other.orthoWindowSize_left_;
    orthoWindowSize_bottom_ = other.orthoWindowSize_bottom_;
    orthoWindowSize_right_ = other.orthoWindowSize_right_;
    orthoWindowSize_top_ = other.orthoWindowSize_top_;


    projectionMatrix_ = other.projectionMatrix_;
    viewMatrix_ = other.viewMatrix_;

    direction_ = other.direction_;
    right_ = other.right_;
    up_ = other.up_;
}

CGCamera::~CGCamera()
{
    if (activeCamera_ == this) {
        activeCamera_ = nullptr;
    }
}

void CGCamera::use()
{
    activeCamera_ = this;
}

void CGCamera::useCamera(CGCamera* camera)
{
    activeCamera_ = camera;
}

CGCamera* CGCamera::getActiveCamera()
{
    return activeCamera_;
}

void CGCamera::setPosition(float x, float y, float z)
{
    position_ = QVector3D(x, y, z);
}

void CGCamera::setPosition(const QVector3D& position)
{
    position_ = position;
}

QVector3D CGCamera::getPosition() const
{
    return position_;
}

void CGCamera::setYaw(float angle)
{
    yaw_ = angle;

    right_ = calculateVectorRight();
}

float CGCamera::getYaw() const
{
    return yaw_;
}

void CGCamera::setRoll(float angle)
{
    roll_ = angle;
}

float CGCamera::getRoll() const
{
    return roll_;
}

void CGCamera::setPitch(float angle)
{
    pitch_ = angle;

    up_ = calculateVectorUp();
}

float CGCamera::getPitch() const
{
    return pitch_;
}

void CGCamera::setDrawDistance(float nearPlane, float farPlane)
{
    nearPlane_ = nearPlane;
    farPlane_ = farPlane;
}

void CGCamera::turnOnPerspective(float verticalAngle, float aspectRatio)
{
    projectionView_ = CGE::PERSPECTIVE;

    verticalAngle_ = verticalAngle;
    aspectRatio_ = aspectRatio;
}

void CGCamera::turnOnOrthography(float left, float bottom, float right, float top)
{
    projectionView_ = CGE::ORTHOGRAPHIC;

    orthoWindowSize_left_ = left;
    orthoWindowSize_bottom_ = bottom;
    orthoWindowSize_right_ = right;
    orthoWindowSize_top_ = top;
}

void CGCamera::refresh()
{
    float _radiansYaw = qDegreesToRadians(yaw_);
    float _radiansPitch = qDegreesToRadians(pitch_);

    direction_.setX(qCos(_radiansYaw) * qCos(_radiansPitch));
    direction_.setY(qSin(_radiansPitch));
    direction_.setZ(qSin(_radiansYaw) * qCos(_radiansPitch));
    direction_.normalize();

    viewMatrix_.setToIdentity();
    viewMatrix_.lookAt(position_, position_ + direction_, up_);
    viewMatrix_.rotate(roll_, direction_);

    projectionMatrix_.setToIdentity();

    if (projectionView_ == CGE::PERSPECTIVE) {
        projectionMatrix_.perspective(verticalAngle_, aspectRatio_, nearPlane_, farPlane_);
    }
    else {
        projectionMatrix_.ortho(orthoWindowSize_left_, orthoWindowSize_right_,
                                orthoWindowSize_bottom_, orthoWindowSize_top_,
                                nearPlane_, farPlane_);
    }
}


QMatrix4x4 CGCamera::getProjectionMatrix() const
{
    return projectionMatrix_;
}

QMatrix4x4 CGCamera::getViewMatrix() const
{
    return viewMatrix_;
}

QVector3D CGCamera::getDirection() const
{
    return direction_;
}

QVector3D CGCamera::getVectorRight() const
{
    return right_;
}

QVector3D CGCamera::getVectorUp() const
{
    return up_;
}

QVector3D CGCamera::calculateVectorRight()
{
    return QVector3D::crossProduct(direction_, up_).normalized();
}

QVector3D CGCamera::calculateVectorUp()
{
    return QVector3D(0.0f, 1.0f, 0.0f).normalized();
}



CGDynamicCamera::CGDynamicCamera()
{
    isFocus_ = -1;

    lastCursorPosition_x_ = 0;
    lastCursorPosition_y_ = 0;

    movementSpeed_ = 1.0f;
    rotationalSpeed_ = 1.0f;
    rollSpeed_ = 1.0f;

    maxPitch_ = -1.0f;
    maxRoll_ = -1.0f;
    maxYaw_ = -1.0f;
}

void CGDynamicCamera::move(CGE::MovementDirection movementDirection)
{
    QVector3D _position = getPosition();

    switch (movementDirection)
    {
        case CGE::MovementDirection::FORWARD:  _position += movementSpeed_ * getDirection();   break;
        case CGE::MovementDirection::BACKWARD: _position -= movementSpeed_ * getDirection();   break;
        case CGE::MovementDirection::RIGHT:    _position += movementSpeed_ * getVectorRight(); break;
        case CGE::MovementDirection::LEFT:     _position -= movementSpeed_ * getVectorRight(); break;
        case CGE::MovementDirection::UP:       _position += movementSpeed_ * getVectorUp();    break;
        case CGE::MovementDirection::DOWN:     _position -= movementSpeed_ * getVectorUp();    break;
    }

    setPosition(_position);
}

void CGDynamicCamera::lookAt(int x, int y)
{
    if (isFocus_ == 1) {
        lastCursorPosition_x_ = x;
        lastCursorPosition_y_ = y;
        isFocus_ = 0;
    }

    int _deltaCursorPos_x_ = x - lastCursorPosition_x_;
    int _deltaCursorPos_y_ = -(y - lastCursorPosition_y_);

    lastCursorPosition_x_ = x;
    lastCursorPosition_y_ = y;

    float _yaw = getYaw();
    float _pitch = getPitch();

    _yaw += rotationalSpeed_ * _deltaCursorPos_x_;
    _pitch += rotationalSpeed_ * _deltaCursorPos_y_;

    if (maxYaw_ >= 0.0f) {
        if (_yaw > maxYaw_)
            _yaw = maxYaw_;

        if (_yaw < -maxYaw_)
            _yaw = -maxYaw_;
    }

    if (maxPitch_ >= 0.0f) {
        if (_pitch > maxPitch_)
            _pitch = maxPitch_;

        if (_pitch < -maxPitch_)
            _pitch = -maxPitch_;
    }

    setYaw(_yaw);
    setPitch(_pitch);
}

void CGDynamicCamera::rotate(float angle)
{
    float _roll = getRoll();

    _roll += rollSpeed_ * angle;

    if (maxRoll_ >= 0.0f) {
        if (_roll > maxRoll_)
            _roll = maxRoll_;

        if (_roll < -maxRoll_)
            _roll = -maxRoll_;
    }

    setRoll(_roll);
}

void CGDynamicCamera::setFocus()
{
    isFocus_ = 1;
}

int CGDynamicCamera::isFocus() const
{
    return isFocus_;
}

void CGDynamicCamera::removeFocus()
{
    isFocus_ = -1;
}

void CGDynamicCamera::setMovementSpeed(float speed)
{
    if (speed < 0.0f)
        return;

    movementSpeed_ = speed;
}

float CGDynamicCamera::getMovementSpeed() const
{
    return movementSpeed_;
}

void CGDynamicCamera::setRotationalSpeed(float speed)
{
    if (speed < 0.0f)
        return;

    rotationalSpeed_ = speed;
}

float CGDynamicCamera::getRotationalSpeed() const
{
    return rotationalSpeed_;
}

void CGDynamicCamera::setRollSpeed(float speed)
{
    if (speed < 0.0f)
        return;

    rollSpeed_ = speed;
}

float CGDynamicCamera::getRollSpeed() const
{
    return rollSpeed_;
}

void CGDynamicCamera::setMaxPitch(float angle)
{
    if (angle < 0 && angle != -1.0f)
        return;

    maxPitch_ = angle;
}

float CGDynamicCamera::getMaxPitch() const
{
    return maxPitch_;
}

void CGDynamicCamera::setMaxRoll(float angle)
{
    if (angle < 0 && angle != -1.0f)
        return;

    maxRoll_ = angle;
}

float CGDynamicCamera::getMaxRoll() const
{
    return maxRoll_;
}

void CGDynamicCamera::setMaxYaw(float angle)
{
    if (angle < 0 && angle != -1.0f)
        return;

    maxYaw_ = angle;
}

float CGDynamicCamera::getMaxYaw() const
{
    return maxYaw_;
}
