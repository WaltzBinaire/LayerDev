#pragma once
#include "ofMain.h"
#include "Layers\layer_collage_manual.h"

class Layer_collage_single : public Layer_collage_manual
{
public:
    Layer_collage_single(string name, int instance, Layer_Manager * _layer_manager)
        : Layer_collage_manual(name, instance, _layer_manager) {}

protected:
    virtual void onRender(bool _forced = false) const  override ;

    void onMousePressed(ofMouseEventArgs & _args);

};

