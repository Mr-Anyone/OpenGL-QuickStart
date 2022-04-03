#include "model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <stbi_image.h>
#include "common.h"

Model::Model(std::string modelPath)
{
    std::cout << "Model path: " << modelPath << std::endl;
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    m_modelDirectory = modelPath.substr(0, modelPath.find_last_of('/'));
    std::cout << m_modelDirectory << std::endl;
    processNode(scene->mRootNode, scene);
    std::cout << "Loaded Model!" << std::endl;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    // prcoces data from current node
    for (int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    // deal with other node
    for (int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<float> vertices{};
    std::vector<unsigned int> indices {};
    std::vector<Texture> textures {};

    for (int i = 0; i < mesh->mNumVertices; ++i)
    {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);
        // x, y, z vertices
        if (mesh->mTextureCoords[0])
        {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            // no texture
            vertices.push_back(0);
            vertices.push_back(0);
        }
    }
    // loading indices 
    for(int i = 0; i<mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i]; 
        for(int j=0; j<face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // load texture
    if(mesh ->mMaterialIndex >= 0)
    {
        // diffuse
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; 
        std::vector<Texture> diffuseMap = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMap.begin(), diffuseMap.end());
        // specular
        std::vector<Texture> specularMap = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMap.begin(), specularMap.end());
    }
    m_meshes.push_back(ModelMesh{vertices, textures, indices});
}

unsigned int textureFromFile(std::string filename);
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string name)
{
    std::vector<Texture> textures {};
    for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);
        std::string texturePath = m_modelDirectory +std::string ("/") + std::string(str.C_Str());
        bool shouldSkip = false; 
        // 
        for(int j =0; j<m_texturesLoaded.size(); ++j)
        {
            if(strcmp(m_texturesLoaded[j].path.c_str(), texturePath.c_str()) == 0)
            {
                textures.push_back(m_texturesLoaded[j]); 
                shouldSkip = true; 
                break;
            }
        }

        if(!shouldSkip)
        {
            Texture texture;
            texture.ID = textureFromFile(texturePath);
            texture.type = name;
            texture.path = texturePath;
            
            textures.push_back(texture);
            m_texturesLoaded.push_back(texture);
        }
    }

    return textures;
}

unsigned int textureFromFile(std::string filename)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

ModelMesh::ModelMesh(const std::vector<float> &vertices, const std::vector<Texture>&  textures, const std::vector<unsigned int>& indices) : 
    m_indicesSize{indices.size()}, m_textures {textures}
{
    loadData(vertices, indices);
}

void ModelMesh::loadData(const std::vector<float> &vertices, const std::vector<unsigned int>& indices)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 6));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // elment array buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW); 
}

void Model::render(const Shader &shader) const
{
    shader.use();
    for (auto &mesh : m_meshes)
    {
        mesh.render(shader);
    }
}

void ModelMesh::render(const Shader &shader) const
{
    shader.use();
    int specularCount{0}, diffuseCount {0};
    for(auto&texture: m_textures)
    {
        glBindTexture(GL_TEXTURE_2D, texture.ID);
        glActiveTexture(GL_TEXTURE0 + (specularCount + diffuseCount));
        
        std::string texture_name;
        if(texture.type == "texture_diffuse")
        {
            texture_name = "texture_diffuse";
            texture_name  += std::to_string(diffuseCount); 
            ++diffuseCount;
        }
        else if (texture.type == "texture_specular")
        {
            texture_name = "texture_specular";
            texture_name  += std::to_string(specularCount); 
            ++specularCount;
        }
        else
        {
            std::cout << "What" << std::endl;
        }
        shader.setInt(texture_name, specularCount + diffuseCount);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, m_indicesSize, GL_UNSIGNED_INT, 0); 
}