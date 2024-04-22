#include "../../includes/objects/camera.hpp"

using namespace obj;

camera::camera()
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

camera::camera(math::vec3 position)
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

camera::camera(math::vec3 position, math::vec3 up, float yaw, float pitch)
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

// constructor with scalar values
camera::camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
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

camera::~camera() {}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
math::mat4	camera::GetViewMatrix()
{
	return math::lookAt(this->Position, this->Position + this->Front, this->Up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void	camera::ProcessKeyboard(camera_movement direction, float deltaTime)
{
	float velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD)
		this->Position += this->Front * velocity;
	if (direction == BACKWARD)
		this->Position -= this->Front * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void	camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw   += xoffset;
	this->Pitch += yoffset;

	// make sure that when Pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void	camera::ProcessMouseScroll(float yoffset)
{
	this->Zoom -= (float)yoffset;
	if (this->Zoom < 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom > 45.0f)
		this->Zoom = 45.0f;
}

// calculates the Front vector from the Camera's (updated) Euler Angles
void	camera::updateCameraVectors()
{
	this->Front.x = std::cos(math::radians(this->Yaw)) * std::cos(math::radians(this->Pitch));
	this->Front.y = std::sin(math::radians(this->Pitch));
	this->Front.z = std::sin(math::radians(this->Yaw)) * std::cos(math::radians(this->Pitch));
	this->Front = math::normalize(this->Front);
	this->Right = math::normalize(math::cross(this->Front, this->WorldUp));
	this->Up    = math::normalize(math::cross(this->Right, this->Front));
	// std::cout << "Front: " << this->Front << std::endl;
	// std::cout << "Right: " << this->Right << std::endl;
	// std::cout << "Up: " << this->Up << std::endl;
}
