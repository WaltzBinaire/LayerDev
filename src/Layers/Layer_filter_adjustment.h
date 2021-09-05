#pragma once
#include "Layers/layer_filter_shader.h"

class Layer_filter_adjustment : public Layer_filter_shader
{
public:
    Layer_filter_adjustment(string name, int instance, Layer_Manager * _layer_manager) : Layer_filter_shader(name, instance, _layer_manager) {};

protected:

    virtual void onDrawGui()     override;
    virtual void onSetupParams() override;

    virtual void setupShader() override;
    virtual void setUniforms(const ofTexture & _baseTex) const override;

    void onResetAdj(bool & _val);
    
    ofParameter<bool> p_resetAdjustments;

    ofParameter<float> p_brightness;
    ofParameter<float> p_contrast;
    ofParameter<float> p_saturation;
    ofParameter<float> p_vibrance;
    ofParameter<float> p_hue;
};

