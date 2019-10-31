#pragma once
#define GLSL410(shader)  "#version 410 \n #define TINY 0.000001 \n" #shader

namespace Shaders {
    string fullscreenVert = GLSL410(
        precision highp float;

        in vec2 position;

        out vec2 v_coordinates;

        void main () {
            v_coordinates = position * 0.5 + 0.5;

            gl_Position = vec4(position, 0.0, 1.0);
        }
    );

    string brushVert = GLSL410(
        precision highp float;
        uniform mat4 modelViewProjectionMatrix;

        in vec2 texcoord;
        out vec2 texcoordVarying;
        uniform sampler2D u_positionsTexture;

        void main () {
           vec3 positionVar = texture2D(u_positionsTexture, texcoord).rgb;

            vec4 outPos = modelViewProjectionMatrix * vec4(positionVar, 1.0);
            outPos.z = 0.0;

            texcoordVarying = texcoord;

            gl_Position = outPos;
        }
    );

    string projectFrag = GLSL410(
        //advects brush positions forward in time

        precision highp float;

        uniform vec2 u_resolution;

        uniform sampler2D u_positionsTexture; 
        uniform sampler2D u_velocitiesTexture;
        uniform sampler2D u_randomsTexture;

        uniform float u_damping;
        uniform float u_gravity;

        uniform float u_verticesPerBristle;

        out vec4 fragColor;

        float random (float n) {
            return fract(0.5 + n * (0.6180339887498949));
        }

        void main () {
            vec2 coordinates = gl_FragCoord.xy / u_resolution;

            vec3 position = texture2D(u_positionsTexture, coordinates).rgb;
            vec3 velocity = texture2D(u_velocitiesTexture, coordinates).rgb;

            velocity *= u_damping;

            velocity += vec3(0.0, 0.0, -u_gravity);
            //velocity += vec3(0.0, 0.0, 0.0 );

            fragColor = vec4(position + velocity, 1.0);
        }
    );

    string distanceconstraintFrag = GLSL410(
        precision highp float;

        uniform int u_pass; //0 or 1, in pass 0 we do 0-1, 2-3, 4-5..., in pass 1 we do 1-2, 3-4, 5-6...

        uniform vec2 u_resolution;

        uniform sampler2D u_positionsTexture;
        uniform float u_pointCount;

        uniform float u_targetDistance;

        out vec4 fragColor;

        void main () {
            vec2 coordinates = gl_FragCoord.xy / u_resolution;

            float index = floor(gl_FragCoord.y); //which vertex on the bristle

            float myWeight = 1.0;
            if (index < 0.1) myWeight = 0.0;

            vec3 myPosition = texture2D(u_positionsTexture, vec2(coordinates.x, (index + 0.5) / u_pointCount)).rgb;

            float otherIndex = 0.0;

            if (mod(index, 2.0) < 0.01) {
                if (u_pass == 0) {
                    otherIndex = index + 1.0;
                } else {
                    otherIndex = index - 1.0;
                }
            } else {
                if (u_pass == 0) {
                    otherIndex = index - 1.0;
                } else {
                    otherIndex = index + 1.0;
                }
            }

            float otherWeight = 1.0;
            if (otherIndex < 0.1) otherWeight = 0.0;

            vec3 newPosition = myPosition;

            if (otherIndex >= 0.0 && otherIndex < u_pointCount) {
                vec3 otherPosition = texture2D(u_positionsTexture, vec2(coordinates.x, (otherIndex + 0.5) / u_pointCount)).rgb;

                float currentDistance = distance(myPosition, otherPosition);
                vec3 towards = (otherPosition - myPosition) / max(currentDistance, 0.01);

                newPosition = myPosition + (myWeight / (myWeight + otherWeight)) * (currentDistance - u_targetDistance) * towards;
            } 

            fragColor = vec4(newPosition, 1.0);

        }
    );
    
    string planeconstraintFrag = GLSL410(
        precision highp float;

        uniform vec2 u_resolution;

        uniform sampler2D u_positionsTexture;

        out vec4 fragColor;

        void main () {
            vec2 coordinates = gl_FragCoord.xy / u_resolution;

            vec3 position = texture2D(u_positionsTexture, coordinates).rgb;

            if (position.z < 0.0) position.z *= 0.5;

            fragColor = vec4(position, 1.0);

        }
    );
    
    string bendingconstraintFrag = GLSL410(
        precision highp float;

        uniform vec2 u_resolution;

        uniform sampler2D u_positionsTexture;
        uniform sampler2D u_randomsTexture;

        uniform float u_pointCount;
        uniform int u_pass; //0, 1 or 2, 0 means we do 0-1-2, 3-4-5, 6-7-8, 1 means we do 1-2-3, 4-5-6, 7-8-9, 2 means we do 2-3-4, 5-6-7, 8-9-10

        uniform float u_stiffnessVariation;

        out vec4 fragColor;

        float length2(vec3 v) {
            return v.x * v.x + v.y * v.y + v.z * v.z;
        }

        void main () {
            vec2 coordinates = gl_FragCoord.xy / u_resolution;

            float index = gl_FragCoord.y;

            float baseIndex = floor((index - float(u_pass)) / 3.0) * 3.0 + float(u_pass); //the base index of this a, b, c pair

            index = floor(index);

            if (baseIndex >= 0.0 && baseIndex < u_pointCount - 2.0) {
                float aIndex = baseIndex;
                float bIndex = baseIndex + 1.0;
                float cIndex = baseIndex + 2.0;

                vec3 aPos = texture2D(u_positionsTexture, vec2(coordinates.x, (aIndex + 0.5) / u_pointCount)).rgb;
                float aW = 1.0;
                if (aIndex < 0.1) aW = 0.0;

                vec3 bPos = texture2D(u_positionsTexture, vec2(coordinates.x, (bIndex + 0.5) / u_pointCount)).rgb;
                float bW = 1.0;

                vec3 cPos = texture2D(u_positionsTexture, vec2(coordinates.x, (cIndex + 0.5) / u_pointCount)).rgb;
                float cW = 1.0;

                vec3 r1 = (bPos - aPos) / distance(aPos, bPos);
                vec3 r2 = (cPos - bPos) / distance(bPos, cPos);

                float constraint = dot(r1, r2) - 1.0;

                float random = texture2D(u_randomsTexture, vec2(coordinates.x, 0.5 / u_pointCount)).g;
                float stiffness = random * u_stiffnessVariation;

                if (constraint > -1.0) {
                    vec3 gradA = (dot(r1, r2) * r1 - r2) / max(distance(aPos, bPos), 0.0001);
                    vec3 gradC = (-dot(r1, r2) * r2 + r1) / max(distance(bPos, cPos), 0.0001);
                    vec3 gradB = -gradA - gradC;

                    float s = stiffness * constraint / max(aW * length2(gradA) + bW * length2(gradB) + cW * length2(gradC), 0.0001);
                    vec3 newPosition;
                    if (index == aIndex) {
                        vec3 currentPosition = aPos;
                        newPosition = currentPosition - s * gradA * aW;
                    } else if (index == bIndex) {
                        vec3 currentPosition = bPos;
                        newPosition = currentPosition - s * gradB * bW;
                    } else if (index == cIndex) {
                        vec3 currentPosition = cPos;
                        newPosition = currentPosition - s * gradC * cW;
                    }

                    //fragColor = vec4(newPosition, 0.0);
                    fragColor = vec4(newPosition, texture2D(u_positionsTexture, coordinates).a);
                } else {
                    fragColor = texture2D(u_positionsTexture, coordinates).rgba;
                }
            } else {
                fragColor = texture2D(u_positionsTexture, coordinates).rgba;
            }
        }
    );
    
    string setbristlesFrag = GLSL410(
        precision highp float;


        uniform vec3  u_brushPosition;
        uniform float u_brushScale;
        uniform float u_bristleCount;

        uniform float u_bristleLength; //length of total bristle
        uniform float u_verticesPerBristle;

        uniform sampler2D u_randomsTexture;
        uniform vec2 u_resolution;

        uniform float u_jitter;

        out vec4 fragColor;

        const float PHI = 1.618033988749895;
        const float PI = 3.14159265;

        void main () {
            vec2 coordinates = gl_FragCoord.xy / u_resolution;

            vec4 randoms = texture2D(u_randomsTexture, coordinates);

            float bristleIndex = floor(gl_FragCoord.x); //which bristle
            float vertexIndex  = floor(gl_FragCoord.y);

            //jittered sunflower distribution

            float theta = (bristleIndex + (randoms.z - 0.5) * u_jitter) * 2.0 * PI / (PHI * PHI);
            float r = sqrt(bristleIndex + 1 + (randoms.w - 0.5) * u_jitter) / sqrt(u_bristleCount);

            float spacing = u_bristleLength / (u_verticesPerBristle - 1.0);
            vec3 brushSpaceBristlePosition = vec3(r * cos(theta), r * sin(theta), -vertexIndex * spacing);

            vec3 bristlePosition = u_brushPosition + brushSpaceBristlePosition * u_brushScale;

            fragColor = vec4(bristlePosition, 1.0);

        }
    );
    
    string updatevelocityFrag = GLSL410(
        //updates velocity with projectedPosition - position

        precision highp float;

        uniform vec2 u_resolution;

        uniform sampler2D u_positionsTexture; 
        uniform sampler2D u_projectedPositionsTexture;

        out vec4 fragColor;

        void main () {
            vec2 coordinates = gl_FragCoord.xy / u_resolution;
            
            vec3 velocity = texture2D(u_projectedPositionsTexture, coordinates).rgb - texture2D(u_positionsTexture, coordinates).rgb;

            fragColor = vec4(velocity, 1.0);

        }
    );

    string brushFrag = GLSL410(
        precision highp float;

        uniform vec4 u_color;
        
        in vec2 texcoordVarying;
        out vec4 fragColor;

        void main () {
            //fragColor = u_color;
            fragColor = vec4( texcoordVarying.y, 0., 0.0, 1.);
        }

    );
}
