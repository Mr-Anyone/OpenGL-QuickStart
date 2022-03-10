#version 330 core
layout(location=0) in vec4 vertex; // <vec 2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 font_projection;

void main() 
{
    gl_Position = font_projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}