#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;

    //cursor position
    currentX = width / 2.0f;
    currentY = height / 2.0f;
    firstClick = true;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(Position, Position + Orientation, Up);

    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow* window)
{
    //movement with keyboard
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += speed * Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position += speed * -Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }

    //mouse rotation inputs
    double mouseX;
    double mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (firstClick)
    {
        currentX = mouseX;
        currentY = mouseY;
        firstClick = false;
    }

    float xOffset = mouseX - currentX;
    float yOffset = currentY - mouseY; 

    currentX = mouseX;
    currentY = mouseY;

    //mouse movement sensitivity
    xOffset *= sensitivity * 0.001f;
    yOffset *= sensitivity * 0.001f;

    glm::vec3 newOrientation = Orientation;

    newOrientation = glm::rotate(newOrientation, glm::radians(-xOffset), Up);

    glm::vec3 rightAxis = glm::normalize(glm::cross(newOrientation, Up));
    newOrientation = glm::rotate(newOrientation, glm::radians(-yOffset), rightAxis);

    float pitch = glm::degrees(asin(newOrientation.y));
    if (pitch > -89.0f && pitch < 89.0f)
    {
        Orientation = newOrientation;
    }

    Orientation.y = 0.0f;
    Orientation = glm::normalize(Orientation);
    // y-axis fixed
    Position.y = 0.0f;


    const float roomBoundary = 4.8f; 
    Position.x = glm::clamp(Position.x, -roomBoundary, roomBoundary);
    Position.z = glm::clamp(Position.z, -roomBoundary, roomBoundary);
}
