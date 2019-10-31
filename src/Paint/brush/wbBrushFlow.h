#pragma once

#include "ofMain.h"
#include "ftUtil.h"
#include "ftFlow.h"
#include "wbBrushShader.h"
#include "ftMouseShader.h"
#include "textureSource\textureSource.h"

#include "fluid\wbDensityAddShader.h"
#include "brush\wbSampleShader.h"
#include "brush\Brush.h"

namespace flowTools {
	
	
	class wbBrushFlow : public ftFlow{
	public:
		
		wbBrushFlow();
		~wbBrushFlow();
		
		void setup(int _width, int _height, ftFlowForceType _type, shared_ptr<Brush> _brush, ofTexture & _colorTexture);
		virtual void update(float _deltaTime, ftPingPongFbo & fbo);
		void reset() override;
		
		ofTexture&			getTexture()		{ return outputFbo.getTexture();}
		ofTexture&			getColorMap()		{ return colorFbo.getTexture();}
		
		ftFlowForceType 	getType()			{ return type; }
		float				getSpeed()			{ return pSpeed.get(); }
		
		void setSpeed(float _value)				{ pSpeed.set(_value); }
		
        bool didChange() const { return true; }

		void setName(string _name)				{ parameters.setName(_name); }
        void setColorTexture(ofTexture & _colorTexture) { colorTexture = _colorTexture; }


        shared_ptr<Brush> brush;


	protected:

        void updateColorMap();

        ofParameter<float>	pSplatRadius; 
		ofParameter<float>	pZThreshold;
		ofParameter<float>		    pSpeed;
		ofParameter<bool>		    pUseColor;
		ofParameter<ofFloatColor>	pColor;

		
		void pTypeListener(int &_value);
		void pPersistentListener(bool &_value) { reset(); }
		
		void drawForce(glm::vec2 _startPosition, glm::vec2 _endPosition);
		
		ftFlowForceType  	type;
		wbBrushShader		brushShader;

        wbDensityAddShader densityAddShader;
        wbSampleShader    sampleShader;
        ofTexture colorTexture;
        ofFbo colorFbo;

		
        bool bChanged;
		void 		saveValue(ftFlowForceType _type, glm::vec4 _force);
		glm::vec4	loadValue(ftFlowForceType _type);
		
	};
}

