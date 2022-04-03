#ifndef MODEL_H 
#define MODEL_H

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#include "shader.h"

class ModelMesh;

class Model
{
public: 
    Model(std::string modelPath);
    void render(const Shader& shader ) const;
private:
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh);
    std::string modelDirectory;
    std::vector<ModelMesh> m_meshes;
};

class ModelMesh
{
public:
    ModelMesh(const std::vector<float>&  vertices);
    void render(const Shader& shader ) const;
private:
    void loadData(const std::vector<float>& vertices);
    unsigned int VAO, VBO;
    std::size_t m_verticesSize;
};
#endif