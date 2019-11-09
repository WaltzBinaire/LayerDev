#include "Layers\layer_collage_single.h"

REGISTER_TYPE(Layer_collage_single, Single Collage)


void Layer_collage_single::onRender(bool _forced) const
{
    collageFbo.begin();

    if(active_patch != nullptr) ofClear(0.0);

    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    collageShader->begin();
    collageShader->setUniform2f("u_alphaRange", p_alphaRange);

    auto quad = LayerUtils::UVQuad::getInstance();

    b_allDrawn = true;
    for(const auto  & colImage : patches) {
        if (colImage->isReady()) {
            if (colImage != active_patch) {
                collageShader->setUniformTexture("tex0", colImage->getTexture(), 0);
                colImage->draw();
                colImage->setDrawn();
            }
        }
        b_allDrawn &= !colImage->isPending() || colImage->isDrawn();
    }
    collageShader->end();
    ofPopStyle();
    collageFbo.end();
}

//--------------------------------------------------------------
void Layer_collage_single::onMousePressed(ofMouseEventArgs & _args)
{
    if (_args.button == 0) {
        switch (mode) {
        case MODE::NONE:
            if (paths.size() > 0) {
                if (patches.size() == 0) {
                    patches.emplace_back(make_shared<CollagePatch>(paths[pathIndex % paths.size()]));
                }

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
