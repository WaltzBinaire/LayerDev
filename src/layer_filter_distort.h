#pragma once
#include "layer_base.h"
#include "layer_filter_shader.h"
#include "pingPongFbo.h"

class Layer_filter_distort : public Layer_filter_shader
{
public:
    Layer_filter_distort(string name, Layer_Manager * _layer_manager) : Layer_filter_shader(name, _layer_manager) {};

    virtual const string get_display_name() const override { return "Distortion"; }

    enum class SHAPE {
        CIRCLE    = 0,
        LINE      = 1,
        RECTANGLE = 2,
        END
    };

protected:
    virtual void onDraw(const ofTexture & _baseTex) const override;
    virtual void onSetup()       override;
    virtual void onReset()       override;
    virtual void onUpdate()      override;
    virtual void onSetupParams() override;
    virtual void onResize()      override;
    virtual void onActivate()    override;
    virtual void onDeactivate()  override;

    void onMousePressed(ofMouseEventArgs & _args);
    void onMouseDragged(ofMouseEventArgs & _args);
    void onMouseReleased(ofMouseEventArgs & _args);
    void onMouseScrolled(ofMouseEventArgs & _args);

    virtual void setupShader() override;
    virtual void setUniforms(const ofTexture & _baseTex) const override;

    void setupBrushFbo();

    bool b_drawing;
    void drawBrush(const ofTexture & _baseTex) const;

    glm::vec2 mousePressedPosition;
    glm::vec2 mousePosition;

    mutable pingPongFbo brushFbo;
    shared_ptr<AutoShader> uv_draw_shader;

    ofParameter<float> p_blur;
    ofParameter<float> p_size;
    ofParameter<int>   p_shape;

private:
    ofEventListener l_onUvShaderLoad;
    mutable bool b_brushFboInitialised;
};

