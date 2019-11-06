#pragma once
#include "Layers\layer_filter_alpha_replace.h"

#ifdef NDEBUG
#include "ofxFaceTracker2.h"
#endif // !NDEBUG


#define MAX_DETECTION_SIZE 720.f

class Layer_alpha_replace_face : public Layer_filter_alpha_replace
{
public:
    Layer_alpha_replace_face(string name, int instance, Layer_Manager * _layer_manager)
        : Layer_filter_alpha_replace(name, instance, _layer_manager) {};


protected:

    virtual void onSetup()  override;
    virtual void onRender(const ofTexture & _baseTex) const override;
    virtual void onDestroy() override;
    virtual void onResize()  override;

    void setupDetectionFbo();

    virtual void setupShader() override;

    virtual void renderReplacmentFbo() const override;
    virtual ofTexture & getAlphaTexture() const override { return faceFbo.getTexture(); };

    void setupFaceFbo();
    void updateFace(const ofTexture & _baseTex) const;

    mutable ofFbo faceFbo;
    shared_ptr<AutoShader> face_shader;
private:
    ofEventListener l_onFaceShaderLoad;


    float getDetectionTextureScale();
    
    float detectionScale;

#ifdef NDEBUG
    mutable ofxFaceTracker2 tracker;
    mutable ofRectangle faceRect;
#endif // !NDEBUG    

    mutable ofPixels pixels;

    mutable ofFbo detectionFbo;
};



