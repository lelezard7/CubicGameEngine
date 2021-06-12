#ifndef _CAMERA_
#define _CAMERA_

#include "glm.hpp"

#define INVERSION_ON    -1
#define INVERSION_OFF    1

enum class CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};


class Camera
{
    glm::vec3 cameraPos_;
    glm::vec3 cameraUp_;
    glm::vec3 cameraDirection_;

    float yaw_;
    float maxYaw_;

    float pitch_;
    float maxPitch_;

    float lastCursorPos_X_;
    float lastCursorPos_Y_;

    int inversion_X_;
    int inversion_Y_;

    float sensitivity_;
    float movementSpeed_;
    float rollSpeed_;

    bool firstCursorMovement = true;

public:
    Camera();

    void setMaxPitch(float value);
    float getMaxPitch();

    void setMaxYaw(float value);
    float getMaxYaw();

    void setPitch(float value);
    float getPitch();

    void setYaw(float value);
    float getYaw();

    bool setInversion_X(int value);
    int getInversion_X();

    bool setInversion_Y(int value);
    int getInversion_Y();

    void setSensitivity(float value);
    float getSensitivity();

    void setMovementSpeed(float value);
    float getMovementSpeed();

    void setRollSpeed(float value);
    float getRollSpeed();

    void setPosition(float x, float y, float z);
    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();

    void watch(float cursorPos_X, float cursorPos_Y, float deltaFrameTime);
    void roll(CameraMovement cameraMovement, float deltaFrameTime, glm::mat4& model);
    void move(CameraMovement cameraMovement, float deltaFrameTime);

    glm::mat4 getViewMatrix();

};

#endif
