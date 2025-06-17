#ifndef WHEEL_H
#define WHEEL_H

#include "GameObject.h"
#include "Mesh.h"
#include <memory>

class Wheel : public GameObject {
private:
    std::unique_ptr<Mesh> mesh;

public:
    Wheel(const glm::vec3& wheelPosition) {
        mesh = std::make_unique<Mesh>();

        // robot wheel settings
        setPosition(wheelPosition);
        setScale(glm::vec3(0.2f, 0.2f, 0.2f)); 
        setColor(glm::vec3(1.0f, 0.0f, 1.0f)); 
    }

    void update(float deltaTime) override {
      
    }

    void render(Shader& shader) override {
        glm::mat4 model = getModelMatrix();
        shader.setMat4("model", glm::value_ptr(model));
        shader.setVec3("objectColor", color);
        mesh->draw();
    }
};

#endif