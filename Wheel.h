#ifndef WHEEL_H
#define WHEEL_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameObject.h"
#include "Mesh.h"
#include <memory>

class Wheel : public GameObject {
private:
    std::unique_ptr<Mesh> mesh;

public:
    Wheel(const glm::vec3& wheelPosition) {
        mesh = std::make_unique<Mesh>(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

        // robot wheel settings
        setPosition(wheelPosition);
        setScale(glm::vec3(0.05f));
    }

    void setColor(const glm::vec3& newColor) override {
        GameObject::setColor(newColor);
        mesh->setColor(glm::vec4(newColor, 1.0f));
    }

    void update(float deltaTime) override {

    }

    void render(Shader& shader) override {
        shader.Activate();
        glm::mat4 model = getModelMatrix();
        shader.setMat4("model", model);
        mesh->draw();
    }
};

#endif

