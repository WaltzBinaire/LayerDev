#version 410
#define TINY 0.000001

uniform mat4 modelViewProjectionMatrix;
in vec4 color;
in vec4 position;


out vec4 fragColor;

void main()
{
    gl_Position = modelViewProjectionMatrix * position;
    fragColor = color;
}