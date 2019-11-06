#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;

in vec4 fragColor;
out vec4 outputColor;

void main()
{
    //vec3 col = fragColor.rgb;    
    //col = vec3( float(col.r > 0.5));
    
    outputColor = vec4(1.0, 1.0, 1.0, 1.0);
}  