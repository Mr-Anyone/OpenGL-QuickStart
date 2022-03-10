#include "font.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream> 
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cassert>

Font::Font(const char* fontPath, const char* vertexShader, const char* fragmentShader, const int width, const int height)
    :m_fontShader {vertexShader, fragmentShader}
{
    std::cout << "Loading Font at " << fontPath << std::endl;
    
    if(FT_Init_FreeType(&m_ft))
    {
        assert(false && "Cannot Initialized Library");
    }

    if(FT_New_Face(m_ft, fontPath, 0, &m_face))
    {
        assert(false && "Failed to load font");
    }
    
    // allocate space for font
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO); 
    glBindVertexArray(VAO); 
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6* 4, NULL, GL_DYNAMIC_DRAW); 

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(0)); 
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // setup shader constant variable 
    m_fontShader.use();
    glm::mat4 projection = glm::ortho(0.f, static_cast<float> (width), 0.f, static_cast<float> (height)); 
    m_fontShader.setMat4("font_projection", projection); 
    m_fontShader.setVec3("textColor", 0.5f, 0.5f, 0.5f);  // set the color as black
    FT_Set_Pixel_Sizes(m_face, 0, 20);
}

void Font::render(const std::string& string, float xPos, float yPos)
{
    glBindVertexArray(VAO); 

    m_fontShader.use();
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for(auto& character: string)
    {
        char renderCharacter {static_cast<char> (character)};
        
        // had is been loaded
        if(m_loadedCharacters.find(renderCharacter) == m_loadedCharacters.end())
        {
            loadCharacter(renderCharacter);
        }
        Character characterInfo = m_loadedCharacters[renderCharacter]; 
        float x = xPos + characterInfo.bearing.x; 
        float y = yPos - (characterInfo.size.y - characterInfo.bearing.y); 

        float vertices [] {
            // vertex position, texture position
            x, y, 0.0, 1.0, 
            x + characterInfo.size.x, y, 1.0, 1.0,
            x, y+ characterInfo.size.y, 0.0, 0.0,

            x + characterInfo.size.x, y, 1.0, 1.0,
            x + characterInfo.size.x, y+ characterInfo.size.y, 1.0, 0.0,
            x, y+ characterInfo.size.y, 0.0, 0.0,

        };

        
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, characterInfo.texutreID); 
        glUniform1i(glGetUniformLocation(m_fontShader.ID, "text"), 0);
        
        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        xPos += characterInfo.advance >> 6;
    }
    glDisable(GL_BLEND);
}

void Font::loadCharacter(char c)
{
    if(FT_Load_Char(m_face, c, FT_LOAD_RENDER))
    {
        assert("Cannot load character" && false);
    }
    Character character; 
    // create texture 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   

    unsigned int textureID; 
    glGenTextures(1, &textureID); 
    glBindTexture(GL_TEXTURE_2D, textureID); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);
    glActiveTexture(GL_TEXTURE0);
    // setting texture warping settings notes(there is no mipmap)    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    character.texutreID = textureID;
    character.bearing = glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top);
    character.size = glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows); 
    character.advance = m_face->glyph->advance.x; 

    m_loadedCharacters.insert(std::pair<char, Character> (c, character));
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);   
}