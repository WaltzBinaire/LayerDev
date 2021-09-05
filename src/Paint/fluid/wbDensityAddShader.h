
#pragma once

#include "ofMain.h"
#include "ftShader.h"

namespace flowTools {
	
	class wbDensityAddShader : public ftShader {
	public:
		wbDensityAddShader() {
            bInitialized = 1;
			if (ofIsGLProgrammableRenderer()) { glFour(); } else { glTwo(); }
			string shaderName = "ftAddMultipiedShader";
			if (bInitialized) { ofLogVerbose(shaderName + " initialized"); }
			else { ofLogWarning(shaderName + " failed to initialize"); }
		}
		
	protected:
		void glTwo() {
			fragmentShader = GLSL120(
									 uniform sampler2DRect baseTex;
									 uniform sampler2DRect blendTex;
									 
									 uniform float	baseWeight;
									 uniform float	blendWeight;
									 uniform vec2	baseScale;
									 uniform vec2	blendScale;
									 
									 void main(){
										 vec2 baseSt = gl_TexCoord[0].st * baseScale;
										 vec2 blendSt = gl_TexCoord[0].st * blendScale;
										 vec4 base = texture2DRect(baseTex, baseSt) * baseWeight;
										 vec4 blend = texture2DRect(blendTex, blendSt) * blendWeight;
										 gl_FragColor = base + blend ;
									 }
									 );
			
			bInitialized *= setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			bInitialized *= linkProgram();
		}
		
		void glFour() {
			fragmentShader = GLSL410(
									 uniform sampler2DRect baseTex;
									 uniform sampler2DRect blendTex;
									 
									 uniform float	baseWeight;
									 uniform float	blendWeight;
									 uniform vec2	baseScale;
									 uniform vec2	blendScale;
									 
									 in vec2 texCoordVarying;
									 out vec4 fragColor;
									 
                                     vec3 rgb2hsb( in vec3 c ){
                                        vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
                                        vec4 p = mix(vec4(c.bg, K.wz),
                                                     vec4(c.gb, K.xy),
                                                     step(c.b, c.g));
                                        vec4 q = mix(vec4(p.xyw, c.r),
                                                     vec4(c.r, p.yzx),
                                                     step(p.x, c.r));
                                        float d = q.x - min(q.w, q.y);
                                        float e = 1.0e-10;
                                        return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),
                                                    d / (q.x + e),
                                                    q.x);
                                    }

                                    //  Function from Iñigo Quiles
                                    //  https://www.shadertoy.com/view/MsS3Wc
                                    vec3 hsb2rgb( in vec3 c ){
                                        vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                                                                 6.0)-3.0)-1.0,
                                                         0.0,
                                                         1.0 );
                                        rgb = rgb*rgb*(3.0-2.0*rgb);
                                        return c.z * mix(vec3(1.0), rgb, c.y);
                                    }

                                    vec3 hsbMix(vec3 a, vec3 b, float t) {
                                        vec3 a_hsb = rgb2hsb(a);
                                        vec3 b_hsb = rgb2hsb(b);
                                        vec3 mix_hsb = vec3(
                                            mix(a_hsb.x, b_hsb.x, t),
                                            mix(a_hsb.y, b_hsb.y, t),
                                            a.z
                                        );
                                        return hsb2rgb(mix_hsb);
                                    }


									 void main(){
										 vec2 baseSt  = texCoordVarying * baseScale;
										 vec2 blendSt = texCoordVarying * blendScale;
										 vec4 base    = texture(baseTex, baseSt)   ;
										 vec4 blend   = texture(blendTex, blendSt);



                                         //float a = mix(blend.a + base.a, pow(blend.a - base.a, 2), 1.0 - base.a); // Cool effect
                                         //float a = mix(blend.a + base.a, pow(blend.a - base.a, 2), blend.a); // Cool effect
                                         //float a = mix( blend.a + base.a, abs(blend.a - base.a), base.a); // Cool effect
                                         //float a = mix( blend.a + base.a, 0.2 + abs(blend.a - base.a), blend.a); 

                                         //float a = base.a + abs(blend.a - base.a); // Cool effect
                                         //float a = (blend.a + blend.b) * max(blend.a, base.a);
                                         //vec3 col =  mix(base.rgb + blend.rgb, blend.rgb, blend.a);
                                         vec3 col =  mix(base.rgb, blend.rgb, blend.a);
                                         float t  = 0.5 + 0.5 * (base.a - blend.a);
                                         //float a  = mix(base.a + blend.a,  base.a, t);

                                         float a;
                                         if (blend.a > 0.) {
                                              a  = blend.a + base.a;
                                              a  = mix( a, 0.666 * (blend.a + base.a), base.a );
                                         }
                                         else {
                                             a = base.a;
                                         }

                                         a = clamp(a, 0.0, 1.0);

                                         fragColor = vec4(col, a);
                                         //fragColor = vec4(1.0, 0.0, 0.0, 1.0);
									 }
									 );
			
			bInitialized *= setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
			bInitialized *= setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			bInitialized *= bindDefaults();
			bInitialized *= linkProgram();
		}
		
	public:
		void update(ofFbo& _fbo, ofTexture& _baseTex, ofTexture& _blendTex, float _baseMultiplier, float _blendMultiplier){
			_fbo.begin();
			begin();
			setUniformTexture("baseTex", _baseTex, 0);
			setUniformTexture("blendTex", _blendTex, 1);
			setUniform1f("baseWeight", _baseMultiplier);
			setUniform1f("blendWeight", _blendMultiplier);
			setUniform2f("baseScale", _baseTex.getWidth() / _fbo.getWidth(), _baseTex.getHeight() / _fbo.getHeight());
			setUniform2f("blendScale", _blendTex.getWidth() / _fbo.getWidth(), _blendTex.getHeight() / _fbo.getHeight());
			renderFrame(_fbo.getWidth(), _fbo.getHeight());
			end();
			_fbo.end();
		}


        
	};
}

