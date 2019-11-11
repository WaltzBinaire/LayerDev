#pragma once
#include "ofMain.h"
#include "Layers\layer_collage.h"

class Layer_collage_manual : public Layer_collage
{
public:
    Layer_collage_manual(string name, int instance, Layer_Manager * _layer_manager)
        : Layer_collage(name, instance, _layer_manager) {}

    int getPathIndex() const { return pathIndex;  }

    virtual string getCursorData() const  override;

protected:

    enum class MODE {NONE, PLACING} mode = MODE::NONE;

    virtual void onSetupListeners()  override ;

    virtual void onMouseMoved(ofMouseEventArgs & _args);
    virtual void onMousePressed(ofMouseEventArgs & _args);
    virtual void onMouseScrolled(ofMouseEventArgs & _args);
    virtual void onKeyPressed(ofKeyEventArgs & _args);

    int pathIndex = 0;

    
    ofEventListener l_onKeyPressed;


    glm::vec2 lastMousePosition;
};

