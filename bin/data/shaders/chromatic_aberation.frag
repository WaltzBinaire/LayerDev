#version 410
#define TINY 0.000001
#define PI 3.14159265358979323846

uniform sampler2D u_imageTex;
uniform vec2 u_amount;
uniform float u_angle;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec3 col;
    float a = texture(u_imageTex, texCoordVarying).a;
       
    vec2 disp = u_amount * vec2( sin(u_angle / 360 * PI ), cos(u_angle / 360 * PI ) );
    col.r = texture(u_imageTex, texCoordVarying - disp).r;
    col.g = texture(u_imageTex, texCoordVarying).g;
    col.b = texture(u_imageTex, texCoordVarying + disp).b;

    outputColor = vec4(col, a);
    
    
}  
