#pragma once
#include "Layers\layer_base.h"

#include "ofxFlowTools.h"
#include "Paint/fluid/wbFluidFlow.h"
#include "wbPaintShader.h"
#include "Paint/brush/Brush.h"

using namespace flowTools;


#define MAX_SIMULATION_SIZE 800
#define MAX_DENSITY_SIZE    1500

class Layer_paint: public Filter_base {
public:
    Layer_paint(string name, int instance, Layer_Manager * _layer_manager)
        : Filter_base(name, instance, _layer_manager) {};


protected:
    virtual void onSetup()          override;
    virtual void onSetupParams()    override;

    virtual void onDraw  (const ofTexture & _baseTex, bool _forced = false) const override;
    virtual void onRender(const ofTexture & _baseTex, bool _forced = false) const override;
    
    virtual void onDrawOverlay()    override;

    virtual void onDrawGui()        override;

    virtual void onReset()          override;
    virtual void onUpdate()         override;
    virtual void onResize()         override;
    virtual void onSetupListeners() override;

    void onMousePressed  (ofMouseEventArgs & _args);
    void onMouseDragged  (ofMouseEventArgs & _args);
    void onMouseMoved    (ofMouseEventArgs & _args);

    void onMouseReleased (ofMouseEventArgs & _args);
    void onMouseScrolled (ofMouseEventArgs & _args);

private:

    int densityWidth, densityHeight, simulationWidth, simulationHeight;

    glm::vec2 brushPosition;
    bool b_mouseDown;

    // Simulation objects
    mutable wbPainting  * paintingShader;
    mutable ofFbo       * paintingFbo;
    mutable Brush       * brush;
	mutable wbFluidFlow	* fluidFlow;   
};

