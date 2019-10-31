#include "wbBrushFlow.h"

namespace flowTools {
	
	wbBrushFlow::wbBrushFlow()
    {
	}
	
	wbBrushFlow::~wbBrushFlow() {
	}
	
	//--------------------------------------------------------------
    void wbBrushFlow::setup(int _width, int _height, ftFlowForceType _type, shared_ptr<Brush> _brush, ofTexture & _colorTexture) 
    {
		if ( _type == FT_OBSTACLE ||
             _type == FT_PRESSURE ||
             _type == FT_TEMPERATURE
            ) 
        {
			ofLogWarning("ftBrushFlow") << "Type " << ftFlowForceNames[_type] << " not supported";
			return;
		}

        brush = _brush;
        type = _type;

        colorFbo.allocate(brush->getBristleCount(), brush->getVertsPerBristle(), GL_RGBA);

        setColorTexture(_colorTexture);

		ftFlow::allocate(_width, _height, ftUtil::getInternalFormatFromType(type));

        brushShader.setup(type, brush->getBristleCount(), brush->getVertsPerBristle(), brush->getSplatsPerSegment());

        parameters.add(pSplatRadius.set("Splat Radius", 0.05   , 0.0, 10.0 ));
        parameters.add(pZThreshold.set( "Z Threshold" , 0.13333, 0.0, 1.0 ));


		switch (type) {
			case FT_DENSITY:
				parameters.setName("brush density");
                parameters.add(pSpeed.set("Speed", 0.0, 0, 1));
                parameters.add(pUseColor.set("use color", true));
				parameters.add(pColor.set("color", ofFloatColor(1.0,1.0,.0,.5)));
				
				break;
			case FT_VELOCITY:
				parameters.setName("brush velocity");
                parameters.add(pSpeed.set("Speed", 0.0, 0, 10));
				break;
			default:
				break;
		}

	}
	
	//--------------------------------------------------------------
	void wbBrushFlow::update(float _deltaTime, ftPingPongFbo & fbo) {
		ftFlow::reset();
        bChanged = false;

        if (!ofGetMousePressed()) return;

        fbo.swap();
        

        float speed;
		switch (type) {
			case FT_DENSITY:
                updateColorMap();

                //speed = pSpeed.get() * ofMap( brush->getSpeed(), 0.0, 100, 1.0, 10.0,  true);
                speed = pSpeed.get(); 

                brushShader.update(fbo.get(), brush, pSplatRadius.get(), pZThreshold.get(), colorFbo.getTexture(), speed);
                break;
			case FT_VELOCITY:
                speed = 1.0;
                brushShader.update(fbo.get(), brush, pSplatRadius.get(), pZThreshold.get(), colorFbo.getTexture(), speed);
			default:
				break;
		}

	}

	//--------------------------------------------------------------
	void wbBrushFlow::reset() {
		switch (type) {
			case FT_DENSITY:
                 ftFlow::reset();
                 //inputFbo.begin() ; ofClear(255,255,255,0.0); inputFbo.end(); 
                 //outputFbo.begin(); ofClear(255,255,255,0.0); outputFbo.end(); 
                break;
			case FT_VELOCITY:                
                ftFlow::reset();
			default:
				break;
		}
        //ftFlow::reset();

		
	}

    void wbBrushFlow::updateColorMap()
    {     
        if (pUseColor) {
            colorFbo.begin();
            ofClear(pColor.get());
            colorFbo.end();
        }
        else {
            sampleShader.update(colorFbo, brush, colorTexture);
        }
        
    }

}

