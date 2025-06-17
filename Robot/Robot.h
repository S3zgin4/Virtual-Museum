#ifndef ROBOT_H
#define ROBOT_H

#include "RobotBody.h"
#include "Wheel.h"
#include "RobotArm.h"
#include <vector>
#include <memory>

class Robot {
private:
    std::unique_ptr<RobotBody> body;
    std::vector<std::unique_ptr<Wheel>> wheels;
    std::unique_ptr<RobotArm> arm;

public:
    Robot() {
        // Create robot body
        body = std::make_unique<RobotBody>();

        // Create wheels 
        float wheelY = -0.55f;  

        glm::vec3 wheelPositions[] = {
            glm::vec3(-0.6f, wheelY,  0.6f), 
            glm::vec3(0.6f, wheelY,  0.6f),  
            glm::vec3(-0.6f, wheelY, -0.6f), 
            glm::vec3(0.6f, wheelY, -0.6f)  
        };

        for (const auto& pos : wheelPositions) {
            wheels.push_back(std::make_unique<Wheel>(pos));
        }

        arm = std::make_unique<RobotArm>();
    }

    void update(float deltaTime) {
        body->update(deltaTime);

        for (auto& wheel : wheels) {
            wheel->update(deltaTime);
        }

        arm->update(deltaTime);
    }

    void render(Shader& shader) {
        body->render(shader);

        for (auto& wheel : wheels) {
            wheel->render(shader);
        }

        arm->render(shader);
    }
    
    void moveForward(float speed) {
       
    }

    void rotate(float angle) {
        
    }
};

#endif