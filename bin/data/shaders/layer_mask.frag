#version 410
#define TINY 0.000001

uniform sampler2D u_imageTex;
uniform sampler2D u_alphaTex;
uniform int       u_invert;
uniform vec2      u_resolution;

in vec2 texCoordVarying;

out vec4 outputColor;

void main()
{
    vec2 _uv     = vec2( texCoordVarying.x, texCoordVarying.y);

    vec4 base   = texture( u_imageTex, _uv);
    float alpha = texture(u_alphaTex,  _uv).r ;
	alpha = clamp(alpha, 0.0, 1.0);

	if(u_invert == 1) alpha = 1.0 - alpha;


    //outputColor = vec4( alpha, alpha, alpha, 1.0 );

    outputColor = vec4( base.rgb, base.a * alpha );
}   
