#include "Layers\layer_collage_manual.h"
#include "Layer_Manager.h"

REGISTER_TYPE(Layer_collage_manual, Manual Collage)

string Layer_collage_manual::getCursorData() const
{
    string str = "";

    if (paths.size() > 0) {
        str += "f " + ofToString(pathIndex + 1) + "\\"  + ofToString( getNumPaths() );
    }

    if (active_patch != nullptr) {
        str += str == "" ? "" : "\n";
        str += "s " + ofToString(active_patch->getScale());
    }
    
    return str;
}

void  Layer_collage_manual::onSetupListeners()
{
    Layer_collage::onSetupListeners();

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
                lastMousePosition = glm::vec2(_args.x, _args.y);
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
            if (paths.size() > 0) {
                patches.emplace_back(make_shared<CollagePatch>(paths[pathIndex % paths.size()]));
                active_patch = patches.back();
                loadImage(active_patch);

                active_patch->setCenter(glm::vec2(_args.x, _args.y));
                lastMousePosition = glm::vec2(_args.x, _args.y);
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
        if (active_patch != nullptr) {
            if (active_patch->isReady()) {
                pathIndex = (pathIndex + 1) % paths.size();
                active_patch->setNewPath(paths[pathIndex]);
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
                if (active_patch != nullptr) {
                    if (active_patch->isReady()) {

                        if ((int)_args.scrollY == -1) {
                            if (pathIndex == 0) pathIndex = paths.size() - 1;
                            else                pathIndex--;              
                        }
                        else if((int)_args.scrollY == 1) {
                            pathIndex++;  
                        }        
                        pathIndex = pathIndex % paths.size();

                        active_patch->setNewPath(paths[pathIndex]);
                        loadImage(active_patch);
                        redraw();
                    }
                }
            }
            else if (ofGetKeyPressed(OF_KEY_ALT)) {
                if (active_patch != nullptr) {
                    //glm::vec2 vector = glm::vec2(_args.x, _args.y) - lastMousePosition;
                    //float angle = atan2(vector.y, vector.x);
                    float rotation = active_patch->getAngle() + _args.scrollY * TWO_PI * 0.02 ;
                    active_patch->setRotation(rotation);
                }
            }
            else {

                if (active_patch != nullptr) {
                    float scale = active_patch->getScale();
                    scale += _args.scrollY * 0.15;
                    scale = max(0.1f, scale);
                    active_patch->setScale(scale);
                    active_patch->setCenter(glm::vec2(_args.x, _args.y));
                    redraw();
                }
            }

            break;
    }
}