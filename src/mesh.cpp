#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "mesh.h"
#include "shader.h"

void Mesh::loadData()
{
    std::cout << "Loading Mesh Data" << std::endl;
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_verticesSize, m_vertices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0) );
    glEnableVertexAttribArray(0);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indicesSize, m_indices, GL_STATIC_DRAW); 

    // unblind Vertex Array
    glBindVertexArray(0);
}

void Mesh::draw() const
{
    glBindVertexArray(VAO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, m_indicesSize, GL_UNSIGNED_INT, 0); 
}

void Mesh::draw(const Shader& shader) const 
{
    shader.use(); 
    glBindVertexArray(VAO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, m_indicesSize, GL_UNSIGNED_INT, 0); 
}

Mesh::Mesh(Mesh&& mesh)
{
    std::cout << "Called Move Constructor" << std::endl; 
    EBO = mesh.EBO;
    VAO = mesh.VAO;
    VBO = mesh.VBO;

    m_indices = mesh.m_indices; 
    mesh.m_indices = nullptr;

    m_vertices = mesh.m_vertices; 
    mesh.m_vertices = nullptr;
}