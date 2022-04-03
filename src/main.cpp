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
#include "font.h"
#include "model.h"

extern const int width_g;
extern const int height_g;

int main()
{ 
    std::cout << "Build Success"<< std::endl;
    GLFWwindow* window {init()};

    Shader shader {"./../res/shader/vertexShader.glsl", "./../res/shader/fragmentShader.glsl"};
    Model model ("./../res/model/backpack/backpack.obj"); 

    Font timesNewRoman {"./../res/fonts/times_new_roman.ttf", "./../res/shader/font_vertexShader.glsl", "./../res/shader/font_fragmentShader.glsl", width_g, height_g};

    while(!glfwWindowShouldClose(window))
    {
        clearBuffer();
        processInput(window);
        
        shader.use();        
        setShaderConstant(shader);
        timesNewRoman.render("Testing", 10, 10);        
        model.render(shader);

        glfwPollEvents(); 
        glfwSwapBuffers(window);
    }
    return 0;
}