#version 410
#define TINY 0.000001

uniform mat4 modelViewProjectionMatrix;
uniform float u_scale;
in vec4 position;

void main()
{
    gl_Position = modelViewProjectionMatrix * (position / u_scale);
}