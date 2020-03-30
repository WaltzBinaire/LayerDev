#pragma once
#include "ofMain.h"
#include "Layers\layer_base.h"

class LayerVisualiser
{
public:
    LayerVisualiser();
    ~LayerVisualiser();


    void draw(const deque<shared_ptr<Layer_base>> & layers) const;

private:


    void setupFbo();

    ofEventListener l_onWindowResized;

    mutable ofFbo fbo;
    mutable ofCamera cam;
    shared_ptr<AutoShader> shader;
};

