#ifndef ROBOTARM_H
#define ROBOTARM_H

#include "GameObject.h"
#include "Mesh.h"
#include <memory>
#include <GLFW/glfw3.h>

class RobotArm : public GameObject {
private:
    std::unique_ptr<Mesh> armMesh;
    std::unique_ptr<Mesh> jointMesh;
    std::unique_ptr<Mesh> endEffectorMesh;

public:
    RobotArm() {
        armMesh = std::make_unique<Mesh>(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
        jointMesh = std::make_unique<Mesh>(glm::vec4(0.5f, 0.0f, 0.5f, 1.0f));
        endEffectorMesh = std::make_unique<Mesh>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    void update(float deltaTime) override {

    }

    void render(Shader& shader) override {
        shader.Activate();
        renderFirstArm(shader);
        renderJoint(shader);
        renderSecondArm(shader);
        renderEndEffector(shader);
    }

private:
    void renderFirstArm(Shader& shader) {
        // 1. 1st Arm Piece (45° inclined)
        glm::mat4 modelArm1 = getModelMatrix();
        modelArm1 = glm::translate(modelArm1, glm::vec3(0.0f, 0.0f, 0.0f));
        modelArm1 = glm::rotate(modelArm1, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelArm1 = glm::translate(modelArm1, glm::vec3(0.0f, 0.5f, 0.0f));
        modelArm1 = glm::scale(modelArm1, glm::vec3(0.2f, 1.0f, 0.2f));

        shader.setMat4("model", modelArm1); // DÜZELTİLDİ
        armMesh->draw();
    }

    void renderJoint(Shader& shader) {
        // 2. Articular Cube 
        glm::mat4 modelJoint = getModelMatrix();
        modelJoint = glm::translate(modelJoint, glm::vec3(0.0f, 0.0f, 0.0f));
        modelJoint = glm::rotate(modelJoint, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelJoint = glm::translate(modelJoint, glm::vec3(0.0f, 1.0f, 0.0f));
        modelJoint = glm::scale(modelJoint, glm::vec3(0.3f, 0.3f, 0.3f));

        shader.setMat4("model", modelJoint); // DÜZELTİLDİ
        jointMesh->draw();
    }

    void renderSecondArm(Shader& shader) {
        // 3. Second Arm Piece (30° inclined)
        glm::mat4 modelJoint = getModelMatrix();
        modelJoint = glm::translate(modelJoint, glm::vec3(0.0f, 0.0f, 0.0f));
        modelJoint = glm::rotate(modelJoint, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelJoint = glm::translate(modelJoint, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 modelArm2 = modelJoint;
        modelArm2 = glm::rotate(modelArm2, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelArm2 = glm::translate(modelArm2, glm::vec3(0.0f, 0.5f, 0.0f));
        modelArm2 = glm::scale(modelArm2, glm::vec3(0.2f, 1.0f, 0.2f));

        shader.setMat4("model", modelArm2); // DÜZELTİLDİ
        armMesh->draw();
    }

    void renderEndEffector(Shader& shader) {
        // 4. For the scanning mechanism (rotating red cube) 
        glm::mat4 modelJoint = getModelMatrix();
        modelJoint = glm::translate(modelJoint, glm::vec3(0.0f, 0.0f, 0.0f));
        modelJoint = glm::rotate(modelJoint, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelJoint = glm::translate(modelJoint, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 modelArm2 = modelJoint;
        modelArm2 = glm::rotate(modelArm2, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelArm2 = glm::translate(modelArm2, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 modelEnd = modelArm2;
        modelEnd = glm::rotate(modelEnd, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        modelEnd = glm::scale(modelEnd, glm::vec3(0.25f, 0.25f, 0.25f));

        shader.setMat4("model", modelEnd); // DÜZELTİLDİ
        endEffectorMesh->draw();
    }
};

#endif