#ifndef ROBOTBODY_H
#define ROBOTBODY_H

#include "GameObject.h"
#include "Mesh.h"
#include <memory>

class RobotBody : public GameObject {
private:
    std::unique_ptr<Mesh> mesh;

public:
    RobotBody() {
        mesh = std::make_unique<Mesh>();

        // robot body settings
        setPosition(glm::vec3(0.0f, 0.0f, 0.0f)); 
        setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        setColor(glm::vec3(0.6f, 0.6f, 0.6f)); 
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