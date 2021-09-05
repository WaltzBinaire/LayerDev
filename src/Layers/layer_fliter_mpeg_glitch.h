#pragma once
#include "Layers/layer_filter_shader.h"

class Layer_filter_mpeg_glitch :
    public Layer_filter_shader
{
public:

    enum EFFECTS {
        NONE             = 0,
        DISPLACE         = 1 << 0,
        DISCOLOR_LINE    = 1 << 1,
        RGB_LINES        = 1 << 2,
        RGB_LINES_VERT   = 1 << 3,
        LUMA             = 1 << 4,
        ALL              = DISPLACE | DISCOLOR_LINE | RGB_LINES | RGB_LINES_VERT | LUMA 
    };

    static const vector<string> & getEffectNames() {
        static const vector<string> effectNames{ 
            "None", 
            "Displace", 
            "Discolour Lines", 
            "RGB Lines",
            "RGB Line Vert",
            "Luma",
            "All"
        };
        return effectNames;
    }

    Layer_filter_mpeg_glitch(string name, int instance, Layer_Manager * _layer_manager) :
        Layer_filter_shader(name, instance, _layer_manager) {}

protected:
    virtual void onSetup()       override;
    virtual void onUpdate()      override;    
    virtual void onDrawGui()     override;
    virtual void onSetupParams() override;

    virtual void setupShader();
    virtual void setUniforms(const ofTexture & _baseTex) const override;

    void createNoiseTexture();

private:

    ofParameter<int>   p_effectFlags;
    ofParameter<float> p_sizeOfKernel;
    ofParameter<float> p_stopAmount;
    ofParameter<float> p_globalStrength;
    ofParameter<bool>  p_forceUpdate;
    ofParameter<bool>  p_greyScale;

    const int noiseTexSize = 128;

    ofTexture noiseTex;

    ofEventListener        l_onForceUpdate;
};

