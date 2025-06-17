#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include"Texture.h"
#include"Camera.h"


const unsigned int width = 800;
const unsigned int height = 800;

// Callback function to adjust the viewport when the window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process keyboard input
void processInput(GLFWwindow* window) {
    // Close window on pressing ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Configure GLFW: OpenGL version 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(width, height, "Virtual Museum Assignment", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set callback for window resize events
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLAD to load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Set the viewport dimensions
    glViewport(0, 0, width, height);


    
    //a cube with positions, colors, texture coordinates, texture flag(1.0=texture, 0.0=no-texture)
    float vertices[] = {
        //10x10x10
        //front(z = -5)
        -5.0f, -5.0f, -5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 0.0f, 0.0f, 1.0f,
         5.0f, -5.0f, -5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 1.0f, 0.0f, 1.0f,
         5.0f,  5.0f, -5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 1.0f, 1.0f, 1.0f,
        -5.0f,  5.0f, -5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 0.0f, 1.0f, 1.0f,

        //back(z = 5)
        -5.0f, -5.0f,  5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 0.0f, 0.0f, 1.0f, 
         5.0f, -5.0f,  5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 1.0f, 0.0f, 1.0f,
         5.0f,  5.0f,  5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 1.0f, 1.0f, 1.0f,
        -5.0f,  5.0f,  5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 0.0f, 1.0f, 1.0f,

        //left(x = -5)
        -5.0f, -5.0f, -5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 0.0f, 0.0f, 1.0f,
        -5.0f, -5.0f,  5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 1.0f, 0.0f, 1.0f,
        -5.0f,  5.0f,  5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 1.0f, 1.0f, 1.0f,
        -5.0f,  5.0f, -5.0f,  0.7f, 0.7f, 0.7f, 1.0f, 0.0f, 1.0f, 1.0f,

        //right(x = 5)
        5.0f, -5.0f, -5.0f,   0.7f, 0.7f, 0.7f, 1.0f, 0.0f, 0.0f, 1.0f,
        5.0f, -5.0f,  5.0f,   0.7f, 0.7f, 0.7f, 1.0f, 1.0f, 0.0f, 1.0f,
        5.0f,  5.0f,  5.0f,   0.7f, 0.7f, 0.7f, 1.0f, 1.0f, 1.0f, 1.0f,
        5.0f,  5.0f, -5.0f,   0.7f, 0.7f, 0.7f, 1.0f, 0.0f, 1.0f, 1.0f,

        //floor(y = -5)
        -5.0f, -5.0f, -5.0f,  0.6f, 0.6f, 0.6f, 1.0f, 0.0f, 0.0f, 0.0f,
         5.0f, -5.0f, -5.0f,  0.6f, 0.6f, 0.6f, 1.0f, 1.0f, 0.0f, 0.0f,
         5.0f, -5.0f,  5.0f,  0.6f, 0.6f, 0.6f, 1.0f, 1.0f, 1.0f, 0.0f,
        -5.0f, -5.0f,  5.0f,  0.6f, 0.6f, 0.6f, 1.0f, 0.0f, 1.0f, 0.0f,  //(notexture-grey)

        //ceiling(y = 5)
        -5.0f,  5.0f, -5.0f,  0.6f, 0.6f, 0.6f, 1.0f, 0.0f, 0.0f, 0.0f,
         5.0f,  5.0f, -5.0f,  0.6f, 0.6f, 0.6f, 1.0f, 1.0f, 0.0f, 0.0f,
         5.0f,  5.0f,  5.0f,  0.6f, 0.6f, 0.6f, 1.0f, 1.0f, 1.0f, 0.0f,
        -5.0f,  5.0f,  5.0f,  0.6f, 0.6f, 0.6f, 1.0f, 0.0f, 1.0f, 0.0f    //(notexture-grey)
    };


    unsigned int indices[] = {
        //front
        0, 3, 2,
        2, 1, 0,

        //back
        4, 5, 6,
        6, 7, 4,

        //left
        8, 9, 10,
        10, 11, 8,

        //right
        12, 15, 14,
        14, 13, 12,

        //floor
        16, 17, 18,
        18, 19, 16,

        //ceiling
        20, 23, 22,
        22, 21, 20
    };

    glEnable(GL_DEPTH_TEST);


    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    //position, color, texture coordinates, and texture flag
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 10 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 4, GL_FLOAT, 10 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 10 * sizeof(float), (void*)(7 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 1, GL_FLOAT, 10 * sizeof(float), (void*)(9 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();


    Texture wall("wall_orijinal.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    shaderProgram.Activate();
    glUniform1i(glGetUniformLocation(shaderProgram.ID, "tex0"), 0);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 0.0f));

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Clear the screen with a dark gray background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //camera matrix
        shaderProgram.Activate();
        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        wall.Bind();


        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    wall.Delete();
    shaderProgram.Delete();

    // Clean up and exit
    glfwTerminate();
    return 0;
}