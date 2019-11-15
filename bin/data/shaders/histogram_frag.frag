#version 410
#define TINY 0.000001

uniform int       u_channel;

in vec2 texCoordVarying;

out vec4 outputColor;

#define RED    0
#define GREEN  1
#define BLUE   2

void main()
{
    vec3 col = vec3(0.0);
	
		
	if      ( u_channel == RED   ) { col = vec3(1.0, 0.2, 0.2);  }
	else if ( u_channel == GREEN ) { col = vec3(0.2, 1.0, 0.2);  }
	else if ( u_channel == BLUE  ) { col = vec3(0.2, 0.2, 1.0);  }

    outputColor = vec4(col, .8);
}  