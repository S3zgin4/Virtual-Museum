#version 330 core

in vec4 color;
out vec4 FragColor;
in vec2 texCoord;
in float texFlag;

uniform sampler2D tex0;

void main()
{
    if (texFlag > 0.5) {
        FragColor = texture(tex0, texCoord) * color;
    } else {
        FragColor = color;
    }
}