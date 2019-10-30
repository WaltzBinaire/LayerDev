#version 410
#define TINY 0.000001

uniform mat4 modelViewProjectionMatrix;

uniform sampler2D u_imageTex;
uniform int       u_channel;
uniform float     u_scale;
uniform float     u_disp;
in vec2 texcoord;
out vec2 texCoordVarying;

#define RED    0
#define GREEN  1
#define BLUE   2

in vec4 position;

void main()
{
    texCoordVarying = texcoord;
    vec4 pos = modelViewProjectionMatrix * position;
	
	float disp  = 0;
	
	if      ( u_channel == RED   ) { disp = texture( u_imageTex, texCoordVarying).r  + 0.0;  }
	else if ( u_channel == GREEN ) { disp = texture( u_imageTex, texCoordVarying).g  + 1.0; }
	else if ( u_channel == BLUE  ) { disp = texture( u_imageTex, texCoordVarying).b  + 2.0; }
	
	pos.y -= disp * u_scale;
	
    gl_Position = pos;
}