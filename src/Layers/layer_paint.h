#pragma once
#include "Layers\layer_filter_shader.h"

#include "ofxFlowTools.h"
#include "fluid/wbFluidFlow.h"
#include "wbPaintShader.h"
#include "brush/Brush.h"


class Layer_paint: public Layer_filter_shader {
public:
    Layer_paint(string name, int instance, Layer_Manager * _layer_manager)
        : Layer_filter_shader(name, instance, _layer_manager) {};


protected:
    virtual void onRender() const;
    virtual void onDrawGui()        override;
    virtual void onSetup()          override;
    virtual void onReset()          override;
    virtual void onUpdate()         override;
    virtual void onSetupParams()    override;
    virtual void onResize()         override;
    virtual void onSetupListeners() override;

    virtual void setupShader() override;

    void onMousePressed(ofMouseEventArgs & _args);
    void onMouseDragged(ofMouseEventArgs & _args);
    void onMouseReleased(ofMouseEventArgs & _args);
    void onMouseScrolled(ofMouseEventArgs & _args);
};

