#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;
uniform vec2 u_alphaRange;

in vec2 texCoordVarying;

out vec4 outputColor;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
    vec4 val = texture(u_imageTex, texCoordVarying);
    vec3 col = val.rgb;
    float a  = val.a;

    vec2 alphaRange = u_alphaRange;

    a = clamp( map(a, alphaRange.x, alphaRange.y, 0.0, 1.0), 0.0, 1.0);

    outputColor = vec4(col, a);
    
}  