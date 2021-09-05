#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;
uniform vec2 u_alphaRange;
uniform vec2 u_resolution;

in vec2 texCoordVarying;

out vec4 outputColor;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
    vec2 d = 5.0 / u_resolution;
    float a     = texture(u_imageTex, texCoordVarying).a;
    
    float a_x0  = texture(u_imageTex, texCoordVarying + vec2( -d.x , 0.0  )).a;
    float a_x1  = texture(u_imageTex, texCoordVarying + vec2( d.x  , 0.0  )).a;
    float a_y0  = texture(u_imageTex, texCoordVarying + vec2( 0.0  , -d.y )).a;
    float a_y1  = texture(u_imageTex, texCoordVarying + vec2( 0.0  , d.y  )).a;
    
    float x = abs( a - a_x0) + abs( a - a_x1) + abs( a - a_y0) + abs( a - a_y1);
    
    vec3 col = vec3(0.0, 1.0, 0.0);

    a = float( x > 0.0);

    outputColor = vec4(col, x);
    
}  