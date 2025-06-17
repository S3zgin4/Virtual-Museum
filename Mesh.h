#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class Mesh {
private:
    unsigned int VBO, VAO;
    std::vector<float> vertices;
    int vertexCount;
    glm::vec4 color;

    void updateVertexColors() {
        for (size_t i = 0; i < vertices.size(); i += 7) {
            vertices[i + 3] = color.r;
            vertices[i + 4] = color.g;
            vertices[i + 5] = color.b;
            vertices[i + 6] = color.a;
        }
    }

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

public:
    // cube mesh constructor
    Mesh(const glm::vec4& meshColor = glm::vec4(1.0f)) : color(meshColor) {

        vertices = {
            // Front face
            -0.5f, -0.5f, -0.5f,  color.r, color.g, color.b, color.a,
             0.5f, -0.5f, -0.5f,  color.r, color.g, color.b, color.a,
             0.5f,  0.5f, -0.5f,  color.r, color.g, color.b, color.a,
             0.5f,  0.5f, -0.5f,  color.r, color.g, color.b, color.a,
            -0.5f,  0.5f, -0.5f,  color.r, color.g, color.b, color.a,
            -0.5f, -0.5f, -0.5f,  color.r, color.g, color.b, color.a,

            -0.5f, -0.5f,  0.5f,  color.r, color.g, color.b, color.a,
             0.5f, -0.5f,  0.5f,  color.r, color.g, color.b, color.a,
             0.5f,  0.5f,  0.5f,  color.r, color.g, color.b, color.a,
             0.5f,  0.5f,  0.5f,  color.r, color.g, color.b, color.a,
            -0.5f,  0.5f,  0.5f,  color.r, color.g, color.b, color.a,
            -0.5f, -0.5f,  0.5f,  color.r, color.g, color.b, color.a,

            -0.5f,  0.5f,  0.5f,  color.r, color.g, color.b, color.a,
            -0.5f,  0.5f, -0.5f,  color.r, color.g, color.b, color.a,
            -0.5f, -0.5f, -0.5f,  color.r, color.g, color.b, color.a,
            -0.5f, -0.5f, -0.5f,  color.r, color.g, color.b, color.a,
            -0.5f, -0.5f,  0.5f,  color.r, color.g, color.b, color.a,
            -0.5f,  0.5f,  0.5f,  color.r, color.g, color.b, color.a,

             0.5f,  0.5f,  0.5f,  color.r, color.g, color.b, color.a,
             0.5f,  0.5f, -0.5f,  color.r, color.g, color.b, color.a,
             0.5f, -0.5f, -0.5f,  color.r, color.g, color.b, color.a,
             0.5f, -0.5f, -0.5f,  color.r, color.g, color.b, color.a,
             0.5f, -0.5f,  0.5f,  color.r, color.g, color.b, color.a,
             0.5f,  0.5f,  0.5f,  color.r, color.g, color.b, color.a,

             -0.5f, -0.5f, -0.5f,  color.r, color.g, color.b, color.a,
              0.5f, -0.5f, -0.5f,  color.r, color.g, color.b, color.a,
              0.5f, -0.5f,  0.5f,  color.r, color.g, color.b, color.a,
              0.5f, -0.5f,  0.5f,  color.r, color.g, color.b, color.a,
             -0.5f, -0.5f,  0.5f,  color.r, color.g, color.b, color.a,
             -0.5f, -0.5f, -0.5f,  color.r, color.g, color.b, color.a,

             -0.5f,  0.5f, -0.5f,  color.r, color.g, color.b, color.a,
              0.5f,  0.5f, -0.5f,  color.r, color.g, color.b, color.a,
              0.5f,  0.5f,  0.5f,  color.r, color.g, color.b, color.a,
              0.5f,  0.5f,  0.5f,  color.r, color.g, color.b, color.a,
             -0.5f,  0.5f,  0.5f,  color.r, color.g, color.b, color.a,
             -0.5f,  0.5f, -0.5f,  color.r, color.g, color.b, color.a
        };

        vertexCount = 36;
        setupMesh();
    }

    void setColor(const glm::vec4& newColor) {
        color = newColor;
        updateVertexColors();
        // Update buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glBindVertexArray(0);
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

#endif