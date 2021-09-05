#pragma once
#include "Layers/layer_filter_shader.h"

class Layer_filter_chromatic_aberation : public Layer_filter_shader
{
public:
    Layer_filter_chromatic_aberation(string name, int instance, Layer_Manager * _layer_manager) : Layer_filter_shader(name, instance, _layer_manager) {};

protected:

    virtual void onDrawGui()     override;
    virtual void onSetupParams() override;

    virtual void setupShader() override;
    virtual void setUniforms(const ofTexture & _baseTex) const override;
    
    ofParameter<float> p_angle;
    ofParameter<float> p_amount;
};

