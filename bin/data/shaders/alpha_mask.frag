#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;
uniform sampler2D u_alphaTex;
uniform sampler2D u_replacementTex;


in vec2 texCoordVarying;

out vec4 outputColor;



void main()
{
    vec2 _uv     = vec2( texCoordVarying.x, texCoordVarying.y);

    vec4 base        = texture( u_imageTex, _uv);
    vec4 alpha       = texture(u_alphaTex, _uv);
    vec4 replacement = texture(u_replacementTex, _uv);

    vec3 col = mix(base.rgb, replacement.rgb, alpha.rgb);    
    outputColor = vec4( col, base.a );

    //outputColor = base;
    //outputColor = alpha;
    //outputColor = replacement;
    //outputColor = vec4(0.0, 1.0, 1.0, 1.0);
    


}   
