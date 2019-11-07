#include "Layers\layer_collage_manual.h"

REGISTER_TYPE(Layer_collage_manual, Manual Collage)

void  Layer_collage_manual::onSetupListeners()
{
    l_onKeyPressed  = ofEvents().keyPressed.newListener( this, & Layer_collage_manual::onKeyPressed );
    
    l_onMousePressed  = layer_manager->canvasMousePressed.newListener ( this, & Layer_collage_manual::onMousePressed );
    l_onMouseMoved    = layer_manager->canvasMouseMoved.newListener   ( this, & Layer_collage_manual::onMouseMoved );
    l_onMouseScrolled = layer_manager->canvasMouseScrolled.newListener( this, & Layer_collage_manual::onMouseScrolled);
}


//--------------------------------------------------------------
void Layer_collage_manual::onMouseMoved(ofMouseEventArgs & _args)
{
    switch (mode) {
        case MODE::NONE:
            break;
        case MODE::PLACING:
            if (active_patch != nullptr) {
                active_patch->setCenter(glm::vec2(_args.x, _args.y));
            }
            break;
    }
}

//--------------------------------------------------------------
void Layer_collage_manual::onMousePressed(ofMouseEventArgs & _args)
{
    if (_args.button == 0) {
        switch (mode) {
        case MODE::NONE:
            if (patches.size() > 0) {
                active_patch = patches.at(activePatchIndex);
                loadImage(active_patch);
                active_patch->setCenter(glm::vec2(_args.x, _args.y));
                mode = MODE::PLACING;
                redraw();
            }            
            break;
        case MODE::PLACING:                                    
            mode = MODE::NONE;
            if (active_patch != nullptr) {
                active_patch = nullptr;
                redraw();
            }
            break;
        }
    }
    else {
        switch (mode) {
        case MODE::NONE:
            break;
        case MODE::PLACING:                        
            mode = MODE::NONE;
            if (active_patch != nullptr) {
                active_patch->unload();
                active_patch = nullptr;
                redraw();
            }
            break;
        }
    }
}
//--------------------------------------------------------------
void Layer_collage_manual::onKeyPressed(ofKeyEventArgs & _args)
{
    if (_args.key == ' ') {
        if (active_patch != nullptr && patches.size() > 1) {
            if (active_patch->isReady()) {
                glm::vec2 pos = active_patch->getCenter();

                active_patch->unload();
                activePatchIndex = (activePatchIndex + 1) % patches.size();
                active_patch = patches.at(activePatchIndex);
            
                active_patch->setCenter(pos);
                loadImage(active_patch);
                redraw();
            }
        }
    }
}

//--------------------------------------------------------------
void Layer_collage_manual::onMouseScrolled(ofMouseEventArgs & _args)
{
    switch (mode) {
        case MODE::NONE:
            break;
        case MODE::PLACING:

            if (ofGetKeyPressed(OF_KEY_CONTROL)) {
                if (active_patch != nullptr && patches.size() > 1) {
                    if (active_patch->isReady()) {
                        active_patch->unload();

                        activePatchIndex = (activePatchIndex + (int)_args.scrollY) % patches.size();
                        active_patch = patches.at(activePatchIndex);
                        active_patch->setCenter(glm::vec2(_args.x, _args.y));
                        loadImage(active_patch);
                        redraw();
                    }
                }
            }
            else {

                if (active_patch != nullptr) {
                    float scale = active_patch->getScale();
                    scale += _args.scrollY * 0.05;
                    scale = max(0.1f, scale);
                    active_patch->setScale(scale);
                    active_patch->setCenter(glm::vec2(_args.x, _args.y));
                    redraw();
                }
            }

            break;
    }
}