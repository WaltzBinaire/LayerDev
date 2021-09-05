#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    outputColor = texture(u_imageTex, texCoordVarying);
}  