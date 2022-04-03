#ifndef MODEL_H 
#define MODEL_H

#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#include "shader.h"

struct Texture 
{ 
    unsigned int ID;
    std::string type;
    std::string path;
};
class ModelMesh;

class Model
{
public: 
    Model(std::string modelPath);
    void render(const Shader& shader ) const;
private:
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string name);
    std::string m_modelDirectory;
    
    std::vector<ModelMesh> m_meshes;
    std::vector<Texture> m_texturesLoaded; 
};

class ModelMesh
{
public:
    ModelMesh(const std::vector<float>&  vertices, const std::vector<Texture>&  textures);
    void render(const Shader& shader ) const;
private:
    void loadData(const std::vector<float>& vertices);
    unsigned int VAO, VBO;
    std::size_t m_verticesSize;
    std::vector<Texture> m_textures {}; 
};
#endif