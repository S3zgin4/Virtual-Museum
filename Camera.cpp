#include "Camera.h"
#include <imgui.h>
#include <GLFW/glfw3.h>

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
    // ImGui ile herhangi bir widget aktifse ya da ImGui penceresi mouse �zerinde ise
    bool imguiInteracting = ImGui::IsAnyItemActive() || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

    if (imguiInteracting)
    {
        // ImGui ile etkile�im var, fare kilidini a� (serbest b�rak)
        mouseLocked = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        firstClick = true; // fare ilk t�klama kontrol�n� s�f�rla

        // Kamera hareketi kapal�, sadece pozisyon s�n�rlar�n� uygula
        Position.y = -2.0f;
        const float roomBoundary = 14.0f;
        Position.x = glm::clamp(Position.x, -roomBoundary, roomBoundary);
        Position.z = glm::clamp(Position.z, -roomBoundary, roomBoundary);

        return; // �leri kamera kodlar�n� �al��t�rma
    }

    // ImGui ile etkile�im yoksa toggle i�in sol t�k kontrol�
    static bool leftMousePrevState = false;
    bool leftMousePressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

    if (leftMousePressed && !leftMousePrevState)
    {
        // Sol t�k yeni bas�ld�, toggle mouseLocked
        mouseLocked = !mouseLocked;

        if (mouseLocked)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Fare kilitli, gizli
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Fare serbest

        firstClick = true; // toggle sonras� ilk click'i s�f�rla
    }

    leftMousePrevState = leftMousePressed; // durumu g�ncelle

    if (!mouseLocked)
    {
        // Fare serbestse kamera hareketi ve d�n�� kapal�
        // Sadece pozisyon s�n�rlar�n� uygula
        Position.y = -2.0f;
        const float roomBoundary = 14.0f;
        Position.x = glm::clamp(Position.x, -roomBoundary, roomBoundary);
        Position.z = glm::clamp(Position.z, -roomBoundary, roomBoundary);

        return;
    }

    // Mouse kilitli ise kameran�n hareket ve d�n�� i�lemleri

    float currentFrame = glfwGetTime();
    static float lastFrame = currentFrame;
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glm::vec3 flatOrientation = glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += speed * deltaTime * flatOrientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position += speed * deltaTime * -glm::normalize(glm::cross(flatOrientation, Up));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position += speed * deltaTime * -flatOrientation;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += speed * deltaTime * glm::normalize(glm::cross(flatOrientation, Up));

    // Fare konumu al
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (firstClick)
    {
        glfwSetCursorPos(window, width / 2, height / 2);
        currentX = width / 2;
        currentY = height / 2;
        firstClick = false;
        return; // �lk frame d�n�� yapmas�n
    }

    float xOffset = static_cast<float>(mouseX - currentX);
    float yOffset = static_cast<float>(mouseY - currentY);

    // Hassasiyet uygulan�r
    xOffset *= sensitivity * 0.001f;
    yOffset *= sensitivity * 0.001f;

    glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-xOffset), Up);
    glm::vec3 rightAxis = glm::normalize(glm::cross(newOrientation, Up));
    newOrientation = glm::rotate(newOrientation, glm::radians(-yOffset), rightAxis);

    float pitch = glm::degrees(asin(newOrientation.y));
    if (pitch > -89.0f && pitch < 89.0f)
        Orientation = glm::normalize(newOrientation);

    // Fareyi ekran ortas�na sabitle
    glfwSetCursorPos(window, width / 2, height / 2);
    currentX = width / 2;
    currentY = height / 2;

    // Kamera y�ksekli�i ve oda s�n�rlar�
    Position.y = -2.0f;
    const float roomBoundary = 14.0f;
    Position.x = glm::clamp(Position.x, -roomBoundary, roomBoundary);
    Position.z = glm::clamp(Position.z, -roomBoundary, roomBoundary);
}



