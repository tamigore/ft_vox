

#ifndef CAMERA_H
#define CAMERA_H

#include "../math/math.hpp"
#include "../../includes/glad/glad.h"

namespace obj
{

enum camera_movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class camera
{
public:
    math::vec3 Position;
    math::vec3 Front;
    math::vec3 Up;
    math::vec3 Right;
    math::vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    camera();
	camera(math::vec3 position);
    camera(math::vec3 position, math::vec3 up, float yaw, float pitch);
    camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	~camera();

    math::mat4 GetViewMatrix();
    void ProcessKeyboard(camera_movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};

} // namespace obj

#endif