#version 410
#define TINY 0.000001

uniform mat4 modelViewProjectionMatrix;

in vec2 texcoord;
out vec2 texCoordVarying;

in vec4 position;
void main()
{
    texCoordVarying =  vec2( texcoord.x, 1.0 -  texcoord.y);
    gl_Position = modelViewProjectionMatrix * position;
}