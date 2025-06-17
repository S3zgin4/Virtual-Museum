#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderClass.h"   

class GameObject {
protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 color;

public:
    GameObject() : position(0.0f), rotation(0.0f), scale(1.0f), color(1.0f) {}

    virtual ~GameObject() = default;


    // transform operations
    void setPosition(const glm::vec3& pos) { position = pos; }
    void setRotation(const glm::vec3& rot) { rotation = rot; }
    void setScale(const glm::vec3& scl) { scale = scl; }
    virtual void setColor(const glm::vec3& col) { color = col; }

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getRotation() const { return rotation; }
    glm::vec3 getScale() const { return scale; }
    glm::vec3 getColor() const { return color; }

    // model matrix calculation
    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }


    // Pure virtual functions
    virtual void update(float deltaTime) = 0;
    virtual void render(Shader& shader) = 0;

};

#endif
