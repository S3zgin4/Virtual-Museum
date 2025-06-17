#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in float aTexFlag;

out vec4 color;
out vec2 texCoord;
out float texFlag;

uniform mat4 camMatrix;

void main()
{
    gl_Position = camMatrix * vec4(aPos, 1.0);
    
    color = aColor;
    
    texCoord = aTex;
    
    texFlag = aTexFlag;
}