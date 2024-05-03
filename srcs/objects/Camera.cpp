#include "objects/Camera.hpp"

using namespace obj;

Camera::Camera()
{
	this->Position = math::vec3(0.0f, 0.0f, 0.0f);
	this->WorldUp = math::vec3(0.0f, 0.0f, -1.0f);
	this->Yaw = YAW;
	this->Pitch = PITCH;
	this->MovementSpeed = SPEED;
	this->MouseSensitivity = SENSITIVITY;
	this->Zoom = ZOOM;
	updateCameraVectors();
}

Camera::Camera(math::vec3 position)
{
	this->Position = position;
	this->WorldUp = math::vec3(0.0f, 1.0f, 0.0f);
	this->Yaw = YAW;
	this->Pitch = PITCH;
	this->Front = math::vec3(0.0f, 0.0f, -1.0f);
	this->MovementSpeed = SPEED;
	this->MouseSensitivity = SENSITIVITY;
	this->Zoom = ZOOM;
	updateCameraVectors();
}

Camera::Camera(math::vec3 position, math::vec3 up, float yaw, float pitch)
{
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->Front = math::vec3(0.0f, 0.0f, -1.0f);
	this->MovementSpeed = SPEED;
	this->MouseSensitivity = SENSITIVITY;
	this->Zoom = ZOOM;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
{
	this->Position = math::vec3(posX, posY, posZ);
	this->WorldUp = math::vec3(upX, upY, upZ);
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->MovementSpeed = SPEED;
	this->MouseSensitivity = SENSITIVITY;
	this->Zoom = ZOOM;
	updateCameraVectors();
}

Camera::~Camera() {}

math::mat4	Camera::GetViewMatrix()
{
	return math::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void	Camera::ProcessKeyboard(camera_movement direction, float deltaTime)
{
	mutex.lock();
	float velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD)
		this->Position += this->Front * velocity;
	if (direction == BACKWARD)
		this->Position -= this->Front * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;
	if (direction == DOWN)
		this->Position -= this->Up * velocity;
	if (direction == UP)
		this->Position += this->Up * velocity;
	mutex.unlock();
}

void	Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw   += xoffset;
	this->Pitch += yoffset;

	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	updateCameraVectors();
}

void	Camera::ProcessMouseScroll(float yoffset)
{
	this->Zoom -= (float)yoffset;
	if (this->Zoom < 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom > 45.0f)
		this->Zoom = 45.0f;
}

void	Camera::updateCameraVectors()
{
	this->Front.x = std::cos(math::radians(this->Yaw)) * std::cos(math::radians(this->Pitch));
	this->Front.y = std::sin(math::radians(this->Pitch));
	this->Front.z = std::sin(math::radians(this->Yaw)) * std::cos(math::radians(this->Pitch));
	this->Front = math::normalize(this->Front);
	this->Right = math::normalize(math::cross(this->Front, this->WorldUp));
	this->Up    = math::normalize(math::cross(this->Right, this->Front));
}
