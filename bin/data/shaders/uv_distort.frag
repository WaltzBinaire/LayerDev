#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;
uniform sampler2D u_uvMap;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec2 _uv = vec2(texCoordVarying.x,  texCoordVarying.y);
    vec2 uvDist  = texture(u_uvMap, _uv).rg; 
    float blend  = texture(u_uvMap, _uv).a; 

    vec2 uv = mix(_uv, uvDist, blend);

    outputColor = texture(u_imageTex, uv);

    
    //outputColor = vec4(uvDist.x,  uvDist.y, uvDist.y, 1.0);

}  