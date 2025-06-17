#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderClass.h"
#include "Texture.h"

class Model {
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 color;
    std::unique_ptr<Texture> texture;

public:
    Model(const char* objPath, const char* texturePath = nullptr) {
        loadOBJ(objPath);
        setupMesh();
        position = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
        rotation = glm::vec3(0.0f);
        color = glm::vec3(0.9f, 0.8f, 0.7f); // Default warm beige color

        if (texturePath) {
            texture = std::make_unique<Texture>(texturePath, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
        }
    }

    void loadOBJ(const char* path) {
        std::vector<glm::vec3> temp_vertices;
        std::vector<glm::vec2> temp_uvs;
        std::vector<glm::vec3> temp_normals;
        std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

        // Vector to store accumulated normals for each vertex
        std::vector<glm::vec3> accumulated_normals;
        std::vector<int> normal_counts;

        std::ifstream file(path);
        if (!file) {
            std::cerr << "Failed to open OBJ file: " << path << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;

            if (type == "v") {
                glm::vec3 vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                temp_vertices.push_back(vertex);
            }
            else if (type == "vt") {
                glm::vec2 uv;
                iss >> uv.x >> uv.y;
                temp_uvs.push_back(uv);
            }
            else if (type == "vn") {
                glm::vec3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                temp_normals.push_back(normal);
            }
            else if (type == "f") {
                std::string vertex1, vertex2, vertex3;
                iss >> vertex1 >> vertex2 >> vertex3;

                std::vector<std::string> v1 = split(vertex1, '/');
                std::vector<std::string> v2 = split(vertex2, '/');
                std::vector<std::string> v3 = split(vertex3, '/');

                vertexIndices.push_back(std::stoi(v1[0]) - 1);
                vertexIndices.push_back(std::stoi(v2[0]) - 1);
                vertexIndices.push_back(std::stoi(v3[0]) - 1);

                if (v1.size() >= 2 && !v1[1].empty()) {
                    uvIndices.push_back(std::stoi(v1[1]) - 1);
                    uvIndices.push_back(std::stoi(v2[1]) - 1);
                    uvIndices.push_back(std::stoi(v3[1]) - 1);
                }

                if (v1.size() >= 3 && !v1[2].empty()) {
                    normalIndices.push_back(std::stoi(v1[2]) - 1);
                    normalIndices.push_back(std::stoi(v2[2]) - 1);
                    normalIndices.push_back(std::stoi(v3[2]) - 1);
                }
            }
        }

        // Initialize accumulated normals array
        accumulated_normals.resize(temp_vertices.size(), glm::vec3(0.0f));
        normal_counts.resize(temp_vertices.size(), 0);

        // First pass: Accumulate normals for each vertex
        for (unsigned int i = 0; i < vertexIndices.size(); i++) {
            unsigned int vertexIndex = vertexIndices[i];

            if (!normalIndices.empty()) {
                unsigned int normalIndex = normalIndices[i];
                glm::vec3 normal = temp_normals[normalIndex];

                accumulated_normals[vertexIndex] += normal;
                normal_counts[vertexIndex]++;
            }
        }

        // Average and normalize the accumulated normals
        for (size_t i = 0; i < accumulated_normals.size(); i++) {
            if (normal_counts[i] > 0) {
                accumulated_normals[i] /= static_cast<float>(normal_counts[i]);
                accumulated_normals[i] = glm::normalize(accumulated_normals[i]);
            }
            else {
                accumulated_normals[i] = glm::vec3(0.0f, 1.0f, 0.0f);
            }
        }

        // Create final vertex data
        vertices.clear();
        indices.clear();

        for (unsigned int i = 0; i < vertexIndices.size(); i++) {
            unsigned int vertexIndex = vertexIndices[i];
            glm::vec3 vertex = temp_vertices[vertexIndex];

            // Position
            vertices.push_back(vertex.x);
            vertices.push_back(vertex.y);
            vertices.push_back(vertex.z);

            // Normal
            glm::vec3 normal = accumulated_normals[vertexIndex];
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            // Texture coordinates
            if (!uvIndices.empty()) {
                unsigned int uvIndex = uvIndices[i];
                glm::vec2 uv = temp_uvs[uvIndex];
                vertices.push_back(uv.x);
                vertices.push_back(uv.y);
            }
            else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            indices.push_back(i);
        }
    }

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex positions (location = 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

        // Normal vectors (location = 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

        // Texture coordinates (location = 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        glBindVertexArray(0);
    }

    void setPosition(const glm::vec3& pos) {
        position = pos;
    }

    void setScale(const glm::vec3& s) {
        scale = s;
    }

    void setRotation(const glm::vec3& rot) {
        rotation = rot;
    }

    glm::vec3 fetchRotation() const {
        return rotation;
    }

    void setColor(const glm::vec3& newColor) {
        color = newColor;
    }

    glm::vec3 fetchPosition() const { return position; }

    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }

    void render(Shader& shader) {
        shader.Activate();

        glm::mat4 model = getModelMatrix();

        shader.setMat4("model", model);
        shader.setVec3("objectColor", color);

        if (texture) {
            texture->Bind();
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            result.push_back(item);
        }
        return result;
    }
};