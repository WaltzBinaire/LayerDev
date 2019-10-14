#include "Layers\layer_file_image.h"

REGISTER_TYPE(Layer_file_image)

void Layer_file_image::onDraw() const
{
    if (img.isAllocated()) {
        glm::vec2 size = scale * glm::vec2(img.getWidth(), img.getHeight());
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        img.draw( position - size * 0.5, size.x, size.y);
        ofPopStyle();
    }
}

void Layer_file_image::onReset()
{
    img.clear();
    scale = 1.0;
    position = glm::vec2(0);
}

void Layer_file_image::handle_file(const string & _path)
{
    if (!img.load(_path)) {
        ofLogWarning(name) << "Could not open image.";
    }
    else {
        ofLogVerbose(name) << "Image loaded.";
        redraw();        
        b_placing = true;
    };
}