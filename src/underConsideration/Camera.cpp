#include "Camera.h"

#include "gtc/matrix_transform.hpp"


Camera::Camera()
{
    cameraPos_ = glm::vec3(1.0f);
    cameraUp_ = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraDirection_ = glm::vec3(0.0f, 0.0f, -1.0f);

    yaw_ = -90.0f;
    maxYaw_ = -1.0f;

    pitch_ = 0.0f;
    maxPitch_ = 89.0f;

    lastCursorPos_X_ = 0.0f;
    lastCursorPos_Y_ = 0.0f;

    inversion_X_ = INVERSION_OFF;
    inversion_Y_ = INVERSION_ON;

    sensitivity_ = 0.0f;
    movementSpeed_ = 0.0f;
    rollSpeed_ = 0.0f;
}

void Camera::setMaxPitch(float value)
{
    maxPitch_ = value;
}

float Camera::getMaxPitch()
{
    return maxPitch_;
}

void Camera::setMaxYaw(float value)
{
    maxYaw_ = value;
}

float Camera::getMaxYaw()
{
    return maxYaw_;
}

void Camera::setPitch(float value)
{
    if (maxPitch_ < 0.0f)
        pitch_ = value;

    if (value > maxPitch_)
        pitch_ = maxPitch_;

    pitch_ = value;
}

float Camera::getPitch()
{
    return pitch_;
}

void Camera::setYaw(float value)
{
    if (maxYaw_ < 0.0f)
        yaw_ = value;

    if (value > maxYaw_)
        yaw_ = maxYaw_;

    yaw_ = value;
}

float Camera::getYaw()
{
    return yaw_;
}

bool Camera::setInversion_X(int value)
{
    switch (value)
    {
    case INVERSION_ON:    inversion_X_ = INVERSION_ON;    return true;
    case INVERSION_OFF:   inversion_X_ = INVERSION_OFF;   return true;

    default:    return false;
    }
}

int Camera::getInversion_X()
{
    return inversion_X_;
}

bool Camera::setInversion_Y(int value)
{
    switch (value)
    {
    case INVERSION_ON:    inversion_Y_ = INVERSION_ON;    return true;
    case INVERSION_OFF:   inversion_Y_ = INVERSION_OFF;   return true;

    default:    return false;
    }
}

int Camera::getInversion_Y()
{
    return inversion_Y_;
}

void Camera::setSensitivity(float value)
{
    if (value < 0.0f)
        value = 0.0f;

    sensitivity_ = value;
}

float Camera::getSensitivity()
{
    return sensitivity_;
}

void Camera::setMovementSpeed(float value)
{
    if (value < 0.0f)
        value = 0.0f;

    movementSpeed_ = value;
}

float Camera::getMovementSpeed()
{
    return movementSpeed_;
}

void Camera::setRollSpeed(float value)
{
    if (value < 0.0f)
        value = 0.0f;

    rollSpeed_ = value;
}

float Camera::getRollSpeed()
{
    return rollSpeed_;
}

void Camera::setPosition(float x, float y, float z)
{
    cameraPos_ = glm::vec3(x, y, z);
}

void Camera::setPosition(glm::vec3 position)
{
    cameraPos_ = position;
}

glm::vec3 Camera::getPosition()
{
    return cameraPos_;
}

void Camera::watch(float cursorPos_X, float cursorPos_Y, float deltaFrameTime)
{
    if (firstCursorMovement) {
        lastCursorPos_X_ = cursorPos_X;
        lastCursorPos_Y_ = cursorPos_Y;
        firstCursorMovement = false;
    }

    float offset_X = inversion_X_ * (cursorPos_X - lastCursorPos_X_);
    float offset_Y = inversion_Y_ * (cursorPos_Y - lastCursorPos_Y_);

    lastCursorPos_X_ = cursorPos_X;
    lastCursorPos_Y_ = cursorPos_Y;

    offset_X *= sensitivity_ * deltaFrameTime;
    offset_Y *= sensitivity_ * deltaFrameTime;

    yaw_   += offset_X;
    pitch_ += offset_Y;

    if (maxPitch_ >= 0) {
        if (pitch_ > maxPitch_) {
            pitch_ = maxPitch_;
        }

        if (pitch_ < -maxPitch_) {
            pitch_ = -maxPitch_;
        }
    }

    if (maxYaw_ >= 0) {
        if (yaw_ > maxYaw_) {
            yaw_ = maxYaw_;
        }

        if (yaw_ < -maxYaw_) {
            yaw_ = -maxYaw_;
        }
    }

    cameraDirection_.x = cos(glm::radians(pitch_)) * cos(glm::radians(yaw_));
    cameraDirection_.y = sin(glm::radians(pitch_));
    cameraDirection_.z = cos(glm::radians(pitch_)) * sin(glm::radians(yaw_));

    cameraDirection_ = glm::normalize(cameraDirection_);
}

void Camera::roll(CameraMovement cameraMovement, float deltaFrameTime, glm::mat4& model)
{
    if (cameraMovement == CameraMovement::LEFT) {
        model = glm::rotate(model, glm::radians(deltaFrameTime * rollSpeed_), glm::vec3(0.0f, 0.0f, 1.0f));
        return;
    }

    if (cameraMovement == CameraMovement::RIGHT) {
        model = glm::rotate(model, glm::radians(-deltaFrameTime * rollSpeed_), glm::vec3(0.0f, 0.0f, 1.0f));
        return;
    }
}

void Camera::move(CameraMovement cameraMovement, float deltaFrameTime)
{
    if (cameraMovement == CameraMovement::FORWARD) {
        cameraPos_ += deltaFrameTime * movementSpeed_ * glm::normalize(cameraDirection_);
        return;
    }

    if (cameraMovement == CameraMovement::BACKWARD) {
        cameraPos_ -= deltaFrameTime * movementSpeed_ * glm::normalize(cameraDirection_);
        return;
    }

    if (cameraMovement == CameraMovement::LEFT) {
        cameraPos_ -= deltaFrameTime * movementSpeed_ * glm::normalize(glm::cross(cameraDirection_, cameraUp_));
        return;
    }

    if (cameraMovement == CameraMovement::RIGHT) {
        cameraPos_ += deltaFrameTime * movementSpeed_ * glm::normalize(glm::cross(cameraDirection_, cameraUp_));
        return;
    }

    if (cameraMovement == CameraMovement::UP) {
        cameraPos_ += deltaFrameTime * movementSpeed_ * glm::normalize(cameraUp_);
        return;
    }

    if (cameraMovement == CameraMovement::DOWN) {
        cameraPos_ -= deltaFrameTime * movementSpeed_ * glm::normalize(cameraUp_);
        return;
    }
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(cameraPos_, cameraPos_ + cameraDirection_, cameraUp_);
}
