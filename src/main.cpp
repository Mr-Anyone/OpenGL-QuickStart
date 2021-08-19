#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "camera.h"
#include "shader.h"
#include "mesh.h"

constexpr int width_g {800}; 
constexpr int height_g {500}; 
Camera camera_g {glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)};
glm::mat4 projection_g {glm::perspective(glm::radians(45.0f), static_cast <float> (width_g) / height_g, 0.1f, 200.0f)};

void init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void processInput(GLFWwindow* window)
{  
    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 

    constexpr float cameraSpeed {3.0f};
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camear_g.moveFront(cameraSpeed);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camear_g.moveFront(-cameraSpeed);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camear_g.moveSide(-cameraSpeed);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camear_g.moveSide(cameraSpeed);
}

void clearBuffer()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);
}

void draw(const Shader& shader, const Mesh& mesh )
{
    shader.use(); 
    
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection_g));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4 (1.0f)));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID,  "view"), 1, GL_FALSE, glm::value_ptr(camear_g.getView()));
    mesh.draw();
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    static double pX, pY; 
    static bool first {true};
    if(first)
    {
        pX = xPos; 
        pY = yPos; 
        first = false;
        return;
    }

    camear_g.rotate(xPos - pX, pY - yPos);
    pX = xPos; 
    pY = yPos;
}
int main()
{ 
    std::cout << "Build Success"<< std::endl;
    init();
    GLFWwindow* window {glfwCreateWindow(width_g, height_g, "OpenGL Window", nullptr, nullptr)};
    if(window == nullptr)
    {
        std::cerr << "Cannot Make Window Context" << std::endl; 
        std::exit(-1);
    }
    glfwMakeContextCurrent(window); 
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(-1);
    }

    glViewport(0, 0, width_g, height_g);
    Shader shader {"./../res/shader/vertexShader.glsl", "./../res/shader/fragmentShader.glsl"};
    float vertices  [] {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };
    unsigned int indices []  {
        0, 1, 2
    }; 

    Mesh triangle {vertices, indices, 9, 3};
    triangle.loadData();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    while(!glfwWindowShouldClose(window))
    {
        // This on only clears color buffer
        clearBuffer();
        processInput(window);
        
        draw(shader, triangle);

        glfwPollEvents(); 
        glfwSwapBuffers(window);
    }
    return 0;
}