#include "model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "common.h"

Model::Model(std::string modelPath)
{
    std::cout << "Model path: " << modelPath << std::endl;
    Assimp::Importer importer; 
    const aiScene* scene  = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    modelDirectory = modelPath.substr(0, modelPath.find_last_of('/')); 
    std::cout << modelDirectory << std::endl;
    processNode(scene->mRootNode, scene);
    std::cout << "Loaded Model!" << std::endl;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // prcoces data from current node
    for(int i=0; i<node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        processMesh(mesh);
    }
    // deal with other node 
    for(int i = 0; i<node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh* mesh)
{
    std::vector<float> vertices {};
    for(int i = 0; i<mesh->mNumVertices; ++i)
    {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        // x, y, z vertices
    }
    m_meshes.push_back(ModelMesh{vertices});
}

ModelMesh::ModelMesh(const std::vector<float>& vertices): 
    m_verticesSize{vertices.size()}
{
    loadData(vertices);
}

void ModelMesh::loadData(const std::vector<float>& vertices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) (0));
    glEnableVertexAttribArray(0);
}

void Model::render(const Shader& shader ) const
{
    shader.use();
    for(auto& mesh: m_meshes)
    {
        mesh.render(shader);
    }
}

void ModelMesh::render(const Shader& shader ) const
{
    shader.use();
    glBindVertexArray(VAO); 
    glDrawArrays(GL_TRIANGLES, 0, m_verticesSize);
}