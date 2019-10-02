#pragma once
#include "layer_filter_alpha_replace.h"

#include "ofxFaceTracker2.h"

class Layer_alpha_replace_face : public Layer_filter_alpha_replace
{
public:
    Layer_alpha_replace_face(string name, Layer_Manager * _layer_manager)
        : Layer_filter_alpha_replace(name, _layer_manager) {};


protected:

    virtual void onSetup() override;
    virtual void onDestroy() override;
    virtual void onDraw(const ofTexture & _baseTex) const override;
    virtual void onSetupParams() override;
    virtual void setupShader() override;

    virtual ofTexture & getAlphaTexture() const override { return faceFbo.getTexture(); };


    void updateFace(const ofTexture & _baseTex) const;

    mutable ofFbo faceFbo;
    shared_ptr<AutoShader> face_shader;
private:
    ofEventListener l_onFaceShaderLoad;
    mutable ofxFaceTracker2 tracker;
    mutable ofPixels pixels;
};

