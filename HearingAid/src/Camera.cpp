#include "Camera.h"
#include <iostream>
using namespace std;

Camera::Camera() : lastFrame(0), deltaTime(0), camSpeed(2.0f) {}

void Camera::setViewPtr(std::shared_ptr<MatrixStack> view)
{
    View = view;
}

void Camera::look() {
    View->loadIdentity();
    View->lookAt(camPos, camPos + camFront, camUp);
}

glm::vec3 Camera::getCameraPos()
{return camPos;}

void Camera::setCamSpeed(float currentFrame, bool sprint)
{
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float speed;
    if (sprint) speed = 15.0f;
    else speed = 5.0f;
    camSpeed = speed * deltaTime;
}

void Camera::moveForward() {
    camPos += camSpeed * camFront;
}

void Camera::moveBack() {
    camPos -= camSpeed * camFront;
}

void Camera::moveRight() {
    camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
}

void Camera::moveLeft() {
    camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
}

void Camera::lookAround(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(front);
}


glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(camPos, camPos + camFront, camUp);
}



