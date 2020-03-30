#pragma once

#include "ofMain.h"
#include "ftShader.h"
#include "Paint/brush/brush.h"


namespace flowTools {
	
    class wbBrushShader : public ftShader {

    public:

        wbBrushShader() {
            
        }

        void setup(ftFlowForceType _type, int maxBristleCount, int vertsPerBristle, int splatsPerSegements) {

            bInitialized = 1;
            type = _type;

            string shaderName = "ftBrushShader";
            if (ofIsGLProgrammableRenderer()) { 
                if (type == FT_DENSITY) {
                    glFour(); 
                }
                else if (type == FT_VELOCITY) {
                    glFourVelocity(); 
                }
                else {
                    ofLogWarning(shaderName + " failed to initialize with tye " + ftFlowForceNames[_type] + " not supported");
                }                
            }
            else { 
                ofLogError() << "Must be GL 4.1"; 
            }
            if (bInitialized) { ofLogVerbose(shaderName + " initialized"); }
            else { ofLogWarning(shaderName + " failed to initialize"); }


            //---------------------------------------------------//
            // Setup Splats                                      //
            //---------------------------------------------------//
            vector<ofFloatColor>    splatCoordinates;
            vector<glm::vec3>    splatVerts;
            vector<unsigned int> splatIndices;

            splatIndex = 0;
            for (int bristle = 0; bristle < maxBristleCount; ++bristle) {
                for (int vertex = 0; vertex < vertsPerBristle - 1; ++vertex) {
                    //we create a quad for each bristle vertex

                    for (int i = 0; i < splatsPerSegements; ++i) {

                        float t = ((float)i + 0.5) / splatsPerSegements;

                        float textureX = ((float)bristle + 0.5) / maxBristleCount;
                        float textureY = ((float)vertex + 0.5 + t) / vertsPerBristle;


                        //bottom left
                        splatCoordinates.push_back( ofFloatColor( textureX, textureY, -1, -1));

                        //bottom right
                        splatCoordinates.push_back( ofFloatColor( textureX, textureY,  1, -1 ));

                        //top right
                        splatCoordinates.push_back( ofFloatColor( textureX, textureY,  1,  1 ));

                        //top left
                        splatCoordinates.push_back( ofFloatColor( textureX, textureY, -1,  1));


                        //indices
                        splatIndices.push_back(splatIndex + 0);
                        splatIndices.push_back(splatIndex + 1);
                        splatIndices.push_back(splatIndex + 2);
                        

                        splatIndices.push_back(splatIndex + 2);
                        splatIndices.push_back(splatIndex + 3);
                        splatIndices.push_back(splatIndex + 0);


                        //Verts
                        splatVerts.push_back(glm::vec3(0));
                        splatVerts.push_back(glm::vec3(0));
                        splatVerts.push_back(glm::vec3(0));
                        splatVerts.push_back(glm::vec3(0));

                        //Verts
                        splatIndex += 4;
                    }
                }
            }
            splatBuffer.setVertexData( &splatVerts[0]      , splatVerts.size()  , GL_STATIC_DRAW);
            splatBuffer.setIndexData(  &splatIndices[0]    , splatIndices.size(), GL_STATIC_DRAW);    
            splatBuffer.setColorData(  &splatCoordinates[0], splatVerts.size()  , GL_STATIC_DRAW);


            isSetup = true;
        }

 protected:


        void glFour() {

            vertexShader =  GLSL410(
                precision highp float;

                in vec4 color;

                uniform vec2  u_fboDimensions;
                uniform vec2  u_screenDimensions;
                uniform float u_splatRadius;
                uniform float u_zThreshold;

                uniform sampler2D u_positionsTexture;
                uniform sampler2D u_previousPositionsTexture;

                out vec2 v_coordinates; //in [-1, 1]

                out vec2 v_previousPosition;
                out vec2 v_position;

                out vec2 v_quadPosition;
                out vec2 v_texCoords;

                void main () {
                    vec2 coordinates = color.zw; //in [-1, 1]
                    v_texCoords      = color.xy;

                    vec3 texposition      = texture2D(u_positionsTexture, v_texCoords).rgb;
                    vec3 previousPosition = texture2D(u_previousPositionsTexture, v_texCoords).rgb;

                    if (texposition.z > u_zThreshold) {
                        texposition      = vec3(100000000.0, 1000000.0, 100000000.0);
                        previousPosition = vec3(100000000.0, 1000000.0, 100000000.0);
                    }

                    vec2 planarPosition = texposition.xy;
                    vec2 previousPlanarPosition = previousPosition.xy;

                    vec2 mid = (planarPosition + previousPlanarPosition) * 0.5;
    

                    float dist = distance(previousPlanarPosition.xy, planarPosition.xy);
                    vec2 direction = (planarPosition - previousPlanarPosition) / dist;
                    vec2 tangent = vec2(-direction.y, direction.x);

                    vec2 finalPosition = mid + coordinates.x * direction * (dist * 0.5 + u_splatRadius) + coordinates.y * tangent * u_splatRadius;

                    v_previousPosition = previousPlanarPosition;
                    v_position         = planarPosition;
                    v_quadPosition     = finalPosition;


                    v_coordinates = coordinates;
                    
                    vec2 ratio = u_fboDimensions / u_screenDimensions;
                    finalPosition *= ratio;

                    vec2 pos = -1.0 + 2.0 * (finalPosition) / u_fboDimensions;
                    gl_Position =  vec4(pos , 0.0, 1.0);
                }
            );


            fragmentShader = GLSL410(
                precision highp float;

                uniform vec4 u_splatColor;

                uniform sampler2DRect u_colorTexture;

                uniform float u_splatRadius;
                uniform float u_strength;

                in vec2 v_previousPosition;
                in vec2 v_position;

                in vec2 v_quadPosition;

                in vec2 v_texCoords;
                in vec2 v_coordinates;

                out vec4 fragColor;

                float distanceToLine(vec2 a, vec2 b, vec2 p) {
                    float dist = distance(a, b);
                    vec2 direction = (b - a) / dist;

                    float projectedDistance = dot(p - a, direction);
                    projectedDistance = clamp(projectedDistance, 0.0, dist);

                    vec2 projectedPosition = a + projectedDistance * direction;

                    return distance(p, projectedPosition);
                }


                void main() {
                    float splatDistance = distanceToLine(v_previousPosition, v_position, v_quadPosition);

                    vec4 color = texture(u_colorTexture, v_texCoords * vec2(100, 10));

                    float multiplier = max(1.0 - splatDistance / u_splatRadius, 0.0) * u_strength;

                    float a = clamp(color.a * multiplier, 0.0, 1.0);

                    fragColor = vec4(color.rgb, a);

                }

            );

            bInitialized *= setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
            bInitialized *= setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
            bInitialized *= bindDefaults();
            bInitialized *= linkProgram();
        }

        void glFourVelocity() {

            vertexShader =  GLSL410(
                precision highp float;

                in vec4 color;

                uniform vec2  u_fboDimensions;
                uniform vec2  u_screenDimensions;
                uniform float u_splatRadius;
                uniform float u_zThreshold;

                uniform sampler2D u_positionsTexture;
                uniform sampler2D u_previousPositionsTexture;

                out vec2 v_coordinates; //in [-1, 1]

                out vec2 v_previousPosition;
                out vec2 v_position;

                out vec2 v_quadPosition;

                uniform sampler2D u_velocitiesTexture;
                uniform sampler2D u_previousVelocitiesTexture;
                
                out vec2 v_velocity;
                out vec2 v_previousVelocity;

                void main() {
                    vec2 coordinates = color.zw;

                    vec3 position         = texture(u_positionsTexture, color.xy).rgb;
                    vec3 previousPosition = texture(u_previousPositionsTexture, color.xy).rgb;

                    if (position.z > u_zThreshold) {
                        position = vec3(100000000.0, 1000000.0, 100000000.0);
                        previousPosition = vec3(100000000.0, 1000000.0, 100000000.0);
                    }

                    vec2 planarPosition = position.xy;
                    vec2 previousPlanarPosition = previousPosition.xy;

                    vec2 mid = (planarPosition + previousPlanarPosition) * 0.5;


                    float dist = distance(previousPlanarPosition.xy, planarPosition.xy);
                    vec2 direction = (planarPosition - previousPlanarPosition) / dist;
                    vec2 tangent = vec2(-direction.y, direction.x);


                    vec2 finalPosition = mid + coordinates.x * direction * (dist * 0.5 + u_splatRadius) + coordinates.y * tangent * u_splatRadius;

                    v_previousPosition = previousPlanarPosition;
                    v_position         = planarPosition;
                    v_quadPosition     = finalPosition;


                    v_coordinates = coordinates;
                    
                    vec2 ratio = u_fboDimensions / u_screenDimensions;
                    finalPosition *= ratio;

                    vec2 pos = -1.0 + 2.0 * (finalPosition) / u_fboDimensions;
                    gl_Position =  vec4(pos , 0.0, 1.0);

                    vec3 velocity         = texture(u_velocitiesTexture, color.xy).rgb;
                    vec3 previousvelocity = texture(u_previousVelocitiesTexture, color.xy).rgb;
                    
                    v_velocity         = ratio * velocity.xy;
                    v_previousVelocity = ratio * previousvelocity.xy;

                }
            );


            fragmentShader = GLSL410(
                precision highp float;

                in vec2 v_previousVelocity;
                in vec2 v_velocity;

                uniform float u_splatRadius;
                uniform float u_strength;

                in vec2 v_previousPosition;
                in vec2 v_position;

                in vec2 v_quadPosition;

                in vec2 v_coordinates;

                out vec4 fragColor;

                float distanceToLine(vec2 a, vec2 b, vec2 p) {
                    float dist = distance(a, b);
                    vec2 direction = (b - a) / dist;

                    float projectedDistance = dot(p - a, direction);
                    projectedDistance = clamp(projectedDistance, 0.0, dist);

                    vec2 projectedPosition = a + projectedDistance * direction;

                    return distance(p, projectedPosition);
                }

                vec2 clampVelocity(vec2 vel) {
                    float MAX_SPEED = 2.0;

                    float speed = length(vel);

                    if (speed > MAX_SPEED) {
                        vel *= MAX_SPEED / speed;
                    }

                    return vel;
                }

                void main() {
                    float splatDistance = distanceToLine(v_previousPosition, v_position, v_quadPosition);

                    float multiplier = max(1.0 - splatDistance / u_splatRadius, 0.0);

                    vec2 velocity = mix(v_previousVelocity, v_velocity, v_coordinates.x * 0.5 + 0.5);

                    fragColor = vec4(clampVelocity(velocity * u_strength), 0.0, multiplier);
                }
            );

            bInitialized *= setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
            bInitialized *= setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
            bInitialized *= bindDefaults();
            bInitialized *= linkProgram();
        }

    public:
        void update(ofFbo& _fbo, ofTexture & _base, glm::vec2 canvasSize, Brush & _brush, float splatRadius, float zThreshold,  float strength = 1.0) {
            ofPushStyle();
            _fbo.begin();

            _base.draw(0, 0, _fbo.getWidth(), _fbo.getHeight());
            ofEnableAlphaBlending();
            begin();


            setUniform2f("u_fboDimensions"   , _fbo.getWidth(), _fbo.getHeight());
            setUniform2f("u_screenDimensions", canvasSize);

            setUniform1f("u_splatRadius", splatRadius);
            setUniform1f("u_zThreshold", zThreshold);

            setUniformTexture("u_positionsTexture",          _brush.getPositionsTexture()         , 0 );
            setUniformTexture("u_previousPositionsTexture",  _brush.getPreviousPositionsTexture() , 1 );
            setUniform1f("u_strength", strength);

            if (type == FT_DENSITY) {            
                glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
                setUniformTexture("u_colorTexture", _brush.getColorTexture(), 2);
            }
            else if (type == FT_VELOCITY) { 
                glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ZERO, GL_ZERO);
                setUniformTexture("u_velocitiesTexture",         _brush.getVelocitiesTexture()        , 2 );
                setUniformTexture("u_previousVelocitiesTexture", _brush.getPreviousVelocitiesTexture(), 3 );
            }
            renderFrame(); 
            end();

            _fbo.end();

           
            ofDisableAlphaBlending();
            ofPopStyle();
        }

        ftFlowForceType type;

    protected:

        		
		void renderFrame(){
            if (!isSetup) {
                ofLogWarning() << "Brush shader not setup";
                return;
            };
            splatBuffer.drawElements(GL_TRIANGLES, splatIndex);
            
		}

        bool isSetup;
        int splatIndex;        
        ofVbo splatBuffer;
    };
}


