#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "common.h"

extern const int width_g;
extern const int height_g;

int main()
{ 
    std::cout << "Build Success"<< std::endl;
    GLFWwindow* window {init()};

    Shader shader {"./../res/shader/vertexShader.glsl", "./../res/shader/fragmentShader.glsl"};
    float vertices  [] {
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    unsigned int indices []  {
        0, 1, 2
    }; 
    Mesh triangle {vertices, indices, sizeof(vertices)/sizeof(float), sizeof(indices) / sizeof(unsigned int)};


    while(!glfwWindowShouldClose(window))
    {
        clearBuffer();
        processInput(window);
        
        draw(shader, triangle);

        glfwPollEvents(); 
        glfwSwapBuffers(window);
    }
    return 0;
}