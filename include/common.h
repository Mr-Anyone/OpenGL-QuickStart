#ifndef COMMON_H 
#define COMMON_H 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mesh.h"
GLFWwindow* init(); 
void processInput(GLFWwindow* window); 
void clearBuffer(); 
void draw(const Shader& shader, const Mesh& mesh); 
void mouseCallback(GLFWwindow* window, double xPos, double yPos);

#endif