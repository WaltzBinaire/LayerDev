#pragma once
#pragma once

#include "ofMain.h"
#include "ftShader.h"


namespace flowTools {
	
    class wbSampleShader : public ftShader {

    public:

        wbSampleShader() {
            bInitialized = 1;
            if (ofIsGLProgrammableRenderer()) { glFour(); }
            else { bInitialized = 0; }
			string shaderName = "wbSampler";
			if (bInitialized) { ofLogVerbose(shaderName + " initialized"); }
			else { ofLogWarning(shaderName + " failed to initialize"); }

        }

 protected:


        void glFour() {

            fragmentShader = GLSL410(
                precision highp float;

                uniform sampler2D u_positionsTexture;
                uniform sampler2D u_colorTexture;

                uniform vec2  u_scale;
                uniform vec2  u_posScale;
                uniform vec2  u_vScale;

                in vec2 texCoordVarying;

                out vec4 fragColor;

                void main() {
                    vec2 uv = texCoordVarying;

                    uv.y = clamp(uv.y * u_vScale.x +  u_vScale.y, 0.0, 1.0);

                    vec2 position = texture(u_positionsTexture, uv  * u_posScale ).xy;
                    

                    vec4 col = texture( u_colorTexture, position * u_scale);
                    fragColor = col ;
                    //fragColor = vec4(position.x, position.y, 0.0, 1.0) ;

                }

            );

            bInitialized *= setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
            bInitialized *= setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
            bInitialized *= bindDefaults();
            bInitialized *= linkProgram();
        }


    public:
        void update(ofFbo& _fbo, ofTexture & _positionsTex, ofTexture & _src, glm::vec2 _vScale) {
            _fbo.begin();

            begin();
            setUniformTexture( "u_positionsTexture" , _positionsTex, 0 );
            setUniformTexture( "u_colorTexture"     , _src         , 1 );

            setUniform2f("u_scale"   , glm::vec2(1.0 / _src.getWidth() , 1.0 /  _src.getHeight()));
            setUniform2f("u_posScale", glm::vec2(1.0 / _positionsTex.getWidth() , 1.0 /  _positionsTex.getHeight()));
            setUniform2f("u_vScale", _vScale);

            renderFrame(_fbo.getWidth(), _fbo.getHeight()); 
            end();

            _fbo.end();
        }

    };
}


