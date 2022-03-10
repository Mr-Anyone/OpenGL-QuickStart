#ifndef FONT_H 
#define FONT_H 

#include <unordered_map>
#include <glm/glm.hpp>
#include <ft2build.h>
#include <string>
#include FT_FREETYPE_H

#include "shader.h"

struct Character 
{
    unsigned int texutreID; 
    glm::ivec2 size; 
    glm::ivec2 bearing; 
    unsigned int advance;
}; 

class Font
{
public: 
    Font(const char* fontPath, const char* vertexShader, const char* fragmentShader, const int widht, const int height);
    void render(const std::string& string, float xPos, float yPos); 
private:
    void loadCharacter(char c);
    
    std::unordered_map<char, Character> m_loadedCharacters {};
    FT_Library m_ft; 
    FT_Face m_face;
    Shader m_fontShader;
    unsigned int VAO, VBO;
};
#endif