#include "wbFluidFlow.h"

namespace flowTools {
    void wbFluidFlow::addDensity(ftPingPongFbo &_dstFbo, glm::vec2 _canvasSize, Brush & _brush, float _strength)
    {
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		_dstFbo.swap();
		brushDensityShader.update(
            _dstFbo.get(), 
            _dstFbo.getBackTexture(), 
            _canvasSize,
            _brush, 
            pSplatRadius * _brush.getScale(), 
            pZThreshold * _brush.getScale(),
            pDensitySpeed);
		ofPopStyle();
    }

    void wbFluidFlow::addVelocity(ftPingPongFbo &_dstFbo, glm::vec2 _canvasSize, Brush & _brush, float _strength)
    {
		ofPushStyle();
		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		_dstFbo.swap();

		brushVelocityShader.update(
            _dstFbo.get(), 
            _dstFbo.getBackTexture(), 
            _canvasSize,
            _brush, 
            pSplatRadius * _brush.getScale(), 
            pZThreshold * _brush.getScale(),
            pVelocitySpeed);

        ofPopStyle();
    }
}