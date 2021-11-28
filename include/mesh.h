#ifndef MESH_H 
#define MESH_H 

#include <cstddef>
#include "shader.h"

// This class could use smart pointer but I have no idea how to use smart pointer properly
class Mesh{
private:
    float* m_vertices; 
    unsigned int* m_indices;
    std::size_t m_verticesSize; 
    std::size_t m_indicesSize;

    unsigned int VAO, VBO, EBO;    
public: 
    Mesh(float* vertices, unsigned int* indices, std::size_t verticeSize, std::size_t indicesSize):
        m_verticesSize{verticeSize}, m_indicesSize {indicesSize}
    {
        // Copying data 
        m_vertices = new float [verticeSize];
        for(int i = 0; i<verticeSize; ++i)
        {
            m_vertices[i] = vertices[i]; 
        }
        
        m_indices = new unsigned int [indicesSize]; 
        for(int i =0; i<indicesSize; ++i)
        {
            m_indices[i] = indices[i];
        }
        this->loadData();
    }
    
    Mesh(const Mesh& mesh) = delete;
    Mesh(Mesh&& mesh);

    ~Mesh()
    {
        if(m_vertices != nullptr)
            delete [] m_vertices; 
        
        if(m_indices != nullptr)
            delete [] m_indices;
    }

    void loadData();
    void draw() const;
    void draw(const Shader& shader) const;

};

#endif