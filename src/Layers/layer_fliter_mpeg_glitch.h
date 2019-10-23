#pragma once
#include "Layers\layer_filter_shader.h"

class Layer_filter_mpeg_glitch :
    public Layer_filter_shader
{
public:
    Layer_filter_mpeg_glitch(string name, int instance, Layer_Manager * _layer_manager) :
        Layer_filter_shader(name, instance, _layer_manager) {}

protected:
    virtual void onSetup()       override;
    virtual void onUpdate()      override;    
    virtual void onDrawGui()     override;
    virtual void onSetupParams() override;

    virtual void setupShader();
    virtual void setUniforms(const ofTexture & _baseTex) const override;

private:

    ofParameter<int>  p_blockSize;
    ofParameter<bool> p_pause;

    const int noiseTexSize = 64;

    ofTexture noiseTex;
};

