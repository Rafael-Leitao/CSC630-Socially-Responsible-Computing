#pragma once

#include "MatrixStack.h"
#include "WindowManager.h"
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	float camSpeed;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	float lastX;
	float lastY;
	float yaw;
	float pitch;

	bool firstMouse = true;

	
	glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	std::shared_ptr<MatrixStack> View;

	bool movingForward;
	bool movingBackward;
	bool movingLeft;
	bool movingRight;

public:
	Camera();
	// get the View ptr for Matrix Stack
	void setViewPtr(std::shared_ptr<MatrixStack> View);
	// uses LookAt to set the current view based on camera position
	void look();
	// getter for camPos
	glm::vec3 getCameraPos();
	// setter for camFront
	void setCameraFront();
	// getter for camFront
	glm::vec3 getCameraFront();
	// setter for camUp
	void setCameraUp();
	// getter for camUp
	glm::vec3 getCameraUp();
	// setter for camSpeed
	void setCamSpeed(float currentFrame, bool sprint);
	// handle mouse look
	void lookAround(GLFWwindow* window, double xpos, double ypos);

	// movement setters
	void moveForward();
	void moveBack();
	void moveRight();
	void moveLeft();

	glm::mat4 getViewMatrix() const;



};
