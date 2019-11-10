#pragma once
#include "Layers\layer_filter_shader.h"
#include "Utils\pingPongFbo.h"

class Layer_filter_distort : public Layer_filter_shader
{
public:
    Layer_filter_distort(string name, int instance, Layer_Manager * _layer_manager) : Layer_filter_shader(name, instance, _layer_manager) {};

    enum class SHAPE {
        CIRCLE     = 0,
        HLINE      = 1,
        VLINE      = 2,
        RECTANGLE  = 3,
    };

    static const vector<string> & getModeNames() {
        static const vector<string> modeNames{ "Circle", "Horizontal Line", "Vertical Line", "Square" };
        return modeNames;
    }

protected:
    virtual void onRender(const ofTexture & _baseTex, bool _forced = false) const override;
    virtual void onDrawGui()        override;
    virtual void onSetup()          override;
    virtual void onDrawOverlay()    override;
    virtual void onReset()          override;
    virtual void onUpdate()         override;
    virtual void onSetupParams()    override;
    virtual void onResize()         override;
    virtual void onSetupListeners() override;

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
    shared_ptr<AutoShader> uv_overlay_shader;

    ofParameter<float> p_blur;
    ofParameter<float> p_size;
    ofParameter<int>   p_shape;

private:
    ofEventListener l_onUvShaderLoad;
    mutable bool b_brushFboInitialised;
};

