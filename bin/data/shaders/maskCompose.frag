#version 410
#define TINY 0.000001

uniform sampler2D u_baseTex;
uniform sampler2D u_maskTex;
uniform int       u_mode;

in vec2 texCoordVarying;

out vec4 outputColor;


#define EQUALS 0
#define AND    1
#define OR     2
#define XOR    3

void main()
{
    vec2 uv     = vec2( texCoordVarying.x, texCoordVarying.y);


    bool base   = texture( u_baseTex, uv).r > 0.5;
    bool mask   = texture( u_maskTex, uv).r > 0.5;

	bool comp;
	switch( u_mode ){
		case EQUALS:
			comp = mask;
			break;
		case AND:
			comp = base && mask;
			break;
		case OR:
			comp = base || mask;
			break;
		case XOR:
			comp = base ^^ mask;
			break;
	
	}
	float fOut = float(comp);
	
	
    outputColor = vec4( fOut , fOut, fOut , 1.0 );
    //outputColor = vec4( uv.x , uv.y , fOut , 1.0 );
    //outputColor = vec4( 1.0 , 0.0 , 0.0 , 1.0 );
}   
