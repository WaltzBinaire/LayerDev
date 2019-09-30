#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;
uniform vec2  u_thresh;
uniform vec4  u_color;
uniform int   u_invert;


in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec2 _uv     = vec2( texCoordVarying.x, texCoordVarying.y);

    vec4  col = texture( u_imageTex, _uv);
    

    vec4 mixCol = normalize(u_color);
    
    float avr = 0.25 * length(col * mixCol);
    
    float val = float(avr > u_thresh.x || avr < u_thresh.y);
    


    if(u_invert == 0) val = 1.0 - val;
    
    outputColor = vec4( val, val, val, 1.0);
    //outputColor = col;

}  
