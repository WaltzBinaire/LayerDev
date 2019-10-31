#pragma once
#include "ofMain.h"
#include "ftShader.h"

namespace flowTools {

	class wbPainting : public ftShader {
	public:
		wbPainting(){
            bInitialized = 1;
			if (ofIsGLProgrammableRenderer()) { glFour(); } else { glTwo(); }
			string shaderName = "wbPainting";
			if (bInitialized) { ofLogVerbose(shaderName + " initialized"); }
			else { ofLogWarning(shaderName + " failed to initialize"); }

            lightDir.set(      "Light Dir"      , glm::vec3(0.0, -1.0, 1.0), glm::vec3(-1), glm::vec3(1));
            normalScale.set(   "Normal Scale"   , 7    , 0, 20   );
            roughness.set(     "Roughness"      , 0.075, 0, 0.5  );
            F0.set(            "F0"             , 0.05 , 0, 0.15 );
            diffuseScale.set(  "Diffuse Scale"  , 0.5  , 0, 2    );
            specularScale.set( "Specular Scale" , 0.15 , 0, 0.5  );


            params.setName("painting shader");
            params.add(
                lightDir,
                normalScale,
                roughness,
                F0,
                diffuseScale,
                specularScale
            );


		}
		
	protected:
		void glTwo() { }
		
		void glFour() {
			fragmentShader = GLSL410(
				uniform sampler2DRect tex0;
                uniform vec2 _size;

                uniform float u_normalScale;

                uniform vec3  u_lightDirection;
                uniform float u_roughness;
                uniform float u_F0;
                uniform float u_diffuseScale;
                uniform float u_specularScale;


				in vec2 texCoordVarying;
				out vec4 fragColor;

                const float PI = 3.14159265;

                float square (float x) {
                    return x * x;
                }

                float saturate (float x) {
                    return clamp(x, 0.0, 1.0);
                }

                float fresnel (float F0, float lDotH) {
                    float f = pow(1.0 - lDotH, 5.0);

                    return (1.0 - F0) * f + F0;
                }

                float GGX (float alpha, float nDotH) {
                    float a2 = square(alpha);

                    return a2 / (PI * square(square(nDotH) * (a2 - 1.0) + 1.0));
                }

                float GGGX (float alpha, float nDotL, float nDotV) {
                    float a2 = square(alpha);

                    float gl = nDotL + sqrt(a2 + (1.0 - a2) * square(nDotL));
                    float gv = nDotV + sqrt(a2 + (1.0 - a2) * square(nDotV));

                    return 1.0 / (gl * gv);
                }

                float getHeight (vec2 coordinates) {
                    return texture(tex0, coordinates).a;
                }


                vec2 computeGradient(vec2 coordinates) { //sobel operator
                    vec2 delta = 1.5 / _size;
                                         
                    float topLeft  = getHeight(coordinates + vec2(-delta.x, delta.y));
                    float top      = getHeight(coordinates + vec2( 0.0    , delta.y));
                    float topRight = getHeight(coordinates + vec2( delta.x, delta.y));
                                         
                    float left  = getHeight(coordinates + vec2(-delta.x, 0.0));
                    float right = getHeight(coordinates + vec2( delta.x, 0.0));
                                         
                    float bottomLeft  = getHeight(coordinates + vec2(-delta.x, -delta.y));
                    float bottom      = getHeight(coordinates + vec2( 0.0    , -delta.y));
                    float bottomRight = getHeight(coordinates + vec2( delta.x, -delta.y));
                                         
                    return vec2(
                        1.0 * topLeft - 1.0 * topRight   + 2.0 * left - 2.0 * right  + 1.0 * bottomLeft - 1.0 * bottomRight,
                       -1.0 * topLeft + 1.0 * bottomLeft - 2.0 * top  + 2.0 * bottom - 1.0 * topRight   + 1.0 * bottomRight);


                }

                float specularBRDF (vec3 lightDirection, vec3 eyeDirection, vec3 normal, float roughness, float F0) {
                    vec3 halfVector = normalize(lightDirection + eyeDirection);

                    float nDotH = saturate(dot(normal, halfVector));
                    float nDotL = saturate(dot(normal, lightDirection));
                    float nDotV = saturate(dot(normal, eyeDirection));
                    float lDotH = saturate(dot(lightDirection, halfVector));

                    float D = GGX(roughness, nDotH);
                    float G = GGGX(roughness, nDotL, nDotV);
                    float F = fresnel(F0, lDotH);

                    return D * G * F;
                }

				void main(){
					vec2 gradient  = computeGradient(texCoordVarying);

                    vec3 normal = normalize(vec3(
                        gradient.x,
                        gradient.y,
                        u_normalScale
                    ));

					vec4 value = texture(tex0, texCoordVarying);

                    vec3 lightDirection = normalize( u_lightDirection);
                    vec3 eyeDirection = normalize( vec3(0.0, 0.0, 1.0));

                    float diffuse = saturate(dot(lightDirection, normal));

                    diffuse = diffuse * u_diffuseScale + (1.0 - u_diffuseScale);


                    float specular = specularBRDF(lightDirection, eyeDirection, normal, u_roughness, u_F0);

                    vec3 color = value.rgb;

                    vec3 surfaceColor = color * diffuse + specular * u_specularScale;


                    float a = clamp(value.a, 0.0, 1.0);
                    //fragColor = vec4( mix(vec3(1.0, 1.0, 1.0), surfaceColor, a), 1.0);
                    fragColor = vec4( surfaceColor, a);

				}
				);
			
			bInitialized *= setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
			bInitialized *= setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			bInitialized *= bindDefaults();
			bInitialized *= linkProgram();
		}
		
	public:
		void update(ofFbo& _drawBuffer, ofTexture& _srcTex){
			ofPushStyle();
			_drawBuffer.begin();
			begin();


            GLint minFilter = _srcTex.texData.minFilter;
            GLint magFilter = _srcTex.texData.magFilter;
            _srcTex.setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

			setUniformTexture( "tex0" , _srcTex, 0 );
			setUniform2f( "_size" , glm::vec2(_srcTex.getWidth(), _srcTex.getHeight()) );

            setUniform1f( "u_normalScale", normalScale / _srcTex.getWidth() );

            setUniform3f( "u_lightDirection", lightDir);
            setUniform1f( "u_roughness"     , roughness);
            setUniform1f( "u_F0"            , F0);
            setUniform1f( "u_diffuseScale"  , diffuseScale);
            setUniform1f( "u_specularScale" , specularScale);


			renderFrame(_drawBuffer.getWidth(), _drawBuffer.getHeight());
			end();
			_drawBuffer.end();



            _srcTex.setTextureMinMagFilter(minFilter, magFilter);
            ofPopStyle();
		}
		

        ofParameterGroup params;
        ofParameter<glm::vec3> lightDir;
        ofParameter<float>     normalScale;
        ofParameter<float>     roughness;
        ofParameter<float>     F0;
        ofParameter<float>     diffuseScale;
        ofParameter<float>     specularScale;

	};
}
