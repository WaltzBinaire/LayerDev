#pragma once
#include "ofMain.h"
#include "Layers\layer_collage.h"

class Layer_collage_manual : public Layer_collage
{
public:
    Layer_collage_manual(string name, int instance, Layer_Manager * _layer_manager)
        : Layer_collage(name, instance, _layer_manager) {}

protected:

    enum class MODE {NONE, PLACING} mode = MODE::NONE;

    virtual void onSetupListeners()  override ;

    void onMouseMoved(ofMouseEventArgs & _args);
    void onMousePressed(ofMouseEventArgs & _args);
    void onMouseScrolled(ofMouseEventArgs & _args);
    void onKeyPressed(ofKeyEventArgs & _args);

    int pathIndex = 0;

    
    ofEventListener l_onKeyPressed;

};

