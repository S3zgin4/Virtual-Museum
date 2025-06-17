#ifndef ROBOTBODY_H
#define ROBOTBODY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameObject.h"
#include "Mesh.h"
#include <memory>

class RobotBody : public GameObject {
private:
    std::unique_ptr<Mesh> mesh;

public:
    RobotBody() {
        mesh = std::make_unique<Mesh>(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

        // robot body settings
        setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        setScale(glm::vec3(0.5f, 0.4f, 0.5f));
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
        shader.setMat4("model", model); // Doðru kullaným
        mesh->draw();
    }
};

#endif