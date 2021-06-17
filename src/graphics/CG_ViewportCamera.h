#ifndef _CG_VIEWPORT_CAMERA_
#define _CG_VIEWPORT_CAMERA_

#include <QMatrix4x4>
#include <QVector4D>
#include <QWidget>
#include <QtMath>
#include <QDebug>


namespace CGE
{
    enum MovementDirection
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    enum Projection
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };
}

//TODO: Доработать roll. Вектора up и right не обновляются при изменении roll.
//TODO: pitch, roll и yaw со временем могут накопить большое значение. Сделать сброс.

class CGCamera
{
    // Задаваемые поля.

    static CGCamera* activeCamera_;

    QVector3D position_;

    float pitch_;
    float roll_;
    float yaw_;

    CGE::Projection projectionView_;

    float verticalAngle_;
    float aspectRatio_;

    float nearPlane_;
    float farPlane_;

    float orthoWindowSize_left_;
    float orthoWindowSize_bottom_;
    float orthoWindowSize_right_;
    float orthoWindowSize_top_;

public:
    CGCamera();
    CGCamera(const CGCamera& other);
    virtual ~CGCamera();

    void use();
    static void useCamera(CGCamera* camera);
    static CGCamera* getActiveCamera();

    void setPosition(float x, float y, float z);
    void setPosition(const QVector3D& position);

    QVector3D getPosition() const;

    void setYaw(float angle);
    float getYaw() const;

    void setRoll(float angle);
    float getRoll() const;

    void setPitch(float angle);
    float getPitch() const;

    void setDrawDistance(float nearPlane, float farPlane);
    void turnOnPerspective(float verticalAngle, float aspectRatio);
    void turnOnOrthography(float left, float bottom, float right, float top);

    void refresh();

private:

    // Вычисляемые поля.

    QMatrix4x4 projectionMatrix_;
    QMatrix4x4 viewMatrix_;

    QVector3D direction_;
    QVector3D right_;
    QVector3D up_;

public:
    QMatrix4x4 getProjectionMatrix() const;
    QMatrix4x4 getViewMatrix() const;

    QVector3D getDirection() const;
    QVector3D getVectorRight() const;
    QVector3D getVectorUp() const;

protected:
    virtual QVector3D calculateVectorRight();
    virtual QVector3D calculateVectorUp();

};


//TODO: Добавить deltaFrameTime для более равномерных поворотов и перемещения.
//TODO: Добавить зум.

class CGDynamicCamera : public CGCamera
{
    int isFocus_;

    int lastCursorPosition_x_;
    int lastCursorPosition_y_;

    float movementSpeed_;
    float rotationalSpeed_;
    float rollSpeed_;

    float maxPitch_;
    float maxRoll_;
    float maxYaw_;

public:
    CGDynamicCamera();
    CGDynamicCamera(const CGDynamicCamera& other);
    ~CGDynamicCamera();

    void move(CGE::MovementDirection movementDirection);
    void lookAt(int x, int y);
    void rotate(float angle);

    void setFocus();
    int isFocus() const;
    void removeFocus();

    void setMovementSpeed(float speed);
    float getMovementSpeed() const;

    void setRotationalSpeed(float speed);
    float getRotationalSpeed() const;

    void setRollSpeed(float speed);
    float getRollSpeed() const;

    void setMaxPitch(float angle);
    float getMaxPitch() const;

    void setMaxRoll(float angle);
    float getMaxRoll() const;

    void setMaxYaw(float angle);
    float getMaxYaw() const;

};

#endif
