#pragma once

#include "ofMain.h"
#include "ofxFlowTools.h"

#include "ftAdvectShader.h"
#include "ftBuoyancyShader.h"
#include "ftDivergenceShader.h"
#include "ftGradientShader.h"
#include "ftJacobiDiffusionShader.h"
#include "ftJacobiPressureShader.h"
#include "ftObstacleOffsetShader.h"
#include "ftVorticityCurlShader.h"
#include "ftVorticityForceShader.h"

#include "ftAddBooleanShader.h"
#include "ftMultiplyForceShader.h"

#include "wbBrushShader.h"
#include "Brush.h"


namespace flowTools {
    class wbFluidFlow : public ftFluidFlow
    {
    public:
        wbFluidFlow() : ftFluidFlow() { 

            parameters.add(pSplatRadius.set  ("Splat Radius"  , 0.05 , 0.0, 0.1 ));
            parameters.add(pZThreshold.set   ( "Z Threshold"  , 0.5  , 0.0, 1.0 ));
            parameters.add(pDensitySpeed.set ("Density Speed" , 0.015, 0.0, 0.1));
            parameters.add(pVelocitySpeed.set("Velocity Speed", 0.007, 0.0, 0.1));

        };

        void setupBrushRenderer(int _brushBristleCount, int _brushVertexPerBristle, int _brushSplatsPerBristle) {
            brushDensityShader.setup(FT_DENSITY,   _brushBristleCount, _brushVertexPerBristle, _brushSplatsPerBristle);
            brushVelocityShader.setup(FT_VELOCITY, _brushBristleCount, _brushVertexPerBristle, _brushSplatsPerBristle);
        
        }

		virtual void resetOutput()	override { 
            outputFbo.begin();
            //ofClear(127, 127, 127, 0.0);
            //ofClear(127, 127, 127, 0.0);
            ofClear(255, 255, 255, 0.0);
            outputFbo.end();
        
        }

        void addDensity( Brush & _brush, glm::vec2 _canvasSize, float _strength = 1.0)  { addDensity(outputFbo, _canvasSize, _brush); }
        void addVelocity(Brush & _brush, glm::vec2 _canvasSize, float _strength = 1.0)  { addVelocity(inputFbo, _canvasSize, _brush); }

        void addDensity (ftPingPongFbo &_dstFbo, glm::vec2 _canvasSize, Brush & _brush, float _strength = 1.0);
        void addVelocity(ftPingPongFbo &_dstFbo, glm::vec2 _canvasSize, Brush & _brush, float _strength = 1.0);


        wbBrushShader brushDensityShader;
        wbBrushShader brushVelocityShader;


        ofParameter<float>	pSplatRadius; 
		ofParameter<float>	pZThreshold;
		ofParameter<float>	pDensitySpeed;
		ofParameter<float>	pVelocitySpeed;
    };
}
