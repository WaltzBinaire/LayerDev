#include "layer_image_advanced.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_image_advanced, Mask Image)


using namespace ofxCv;
using namespace cv;

void Layer_image_advanced::onSetup()
{
    finder.setup("models/haarcascade_frontalface_default.xml");
    finder.setPreset(ObjectFinder::Accurate);

    setupCustomMask();
}

void Layer_image_advanced::onDrawGui()
{
    SingleLayerGui::specialisedDrawGui<Layer_image_advanced>(this); 
}

void Layer_image_advanced::loadBodyMask(const string & path) { 
    if (img.isAllocated()) {
        setupBodyMask(path);
    }
    else {
        ofLogError(__FUNCDNAME__) << "Image should be loaded before body mask";
    }
}

void Layer_image_advanced::handle_file(const string & _path)
{
    Layer_file_image::handle_file(_path);

    if (img.isAllocated()) {
        setupFaceMask();
    }
}

void Layer_image_advanced::onRender() const
{
    if (needsRedraw() ) {
        clearMaskFbo();
        maskFbo.begin();

        for (auto & maskEl : masks) {
            Mask * mask = maskEl.second;
            if (mask->isActive()) {
                
                QuadMask * quadMask = dynamic_cast<QuadMask *>(mask);
                if (quadMask != nullptr) {
                    quadMask->draw(position, scale);
                }
                StretchMask * stretchMask = dynamic_cast<StretchMask *>(mask);
                if (stretchMask != nullptr) {
                    stretchMask->draw();
                }
            }
        }
        maskFbo.end();
    }
}

void Layer_image_advanced::onResize()
{
    customMask.resize(size);
}

void Layer_image_advanced::setupFaceMask()
{
    finder.update(img);

    faceMask.setup(img.getWidth(), img.getHeight());

    faceMask.begin();
    for (int i = 0; i < finder.size(); i++) {
        auto rect = finder.getObject(i);
        ofSetColor(ofColor::white);
        ofDrawRectangle(rect);
    }

    faceMask.end();

    masks.insert_or_assign("Face Mask", &faceMask);

}

void Layer_image_advanced::setupCustomMask()
{
    customMask.setup(size.x, size.y);
    ofAddListener(customMask.onSetActive, this, &Layer_image_advanced::onCustomMaskActive);
    masks.insert_or_assign("Custom Mask", &customMask);
}

void Layer_image_advanced::setupBodyMask(const string & _path)
{
    ofImage bodyMaskImg(_path); 

    if (!bodyMaskImg.isAllocated()) {
        ofLogError(__FUNCTION__) << "Could not load image: " << _path;
        return;
    }

    float img_aspect = img.getWidth() / img.getHeight();
    float mask_aspect = bodyMaskImg.getWidth() / bodyMaskImg.getHeight();

    if (abs(img_aspect - mask_aspect) > 0.001) {
        ofLogWarning(__FUNCDNAME__) << "Image and image mask should have the same aspect ration";
    }

    bodyMask.setup(img.getWidth(), img.getHeight());

    bodyMask.begin();
    bodyMaskImg.draw(0, 0, img.getWidth(), img.getHeight());
    bodyMask.end();

    masks.insert_or_assign("Body Mask", &bodyMask);
}

void Layer_image_advanced::onCustomMousePressed(ofMouseEventArgs & _args)
{
    drawBrush(_args);
}

void Layer_image_advanced::drawBrush(ofMouseEventArgs & _args)
{
    drawBrush(_args);
}

void Layer_image_advanced::onCustomMouseDragged(ofMouseEventArgs & _args)
{
    customMask.begin();
    brushPosition = glm::vec2(_args.x, _args.y);

    customMask.end();
}

void Layer_image_advanced::onCustomMouseScrolled(ofMouseEventArgs & _args)
{
    brushSize += 5.0 * _args.scrollY;
    brushSize = ofClamp(brushSize, minBrushSize, maxBrushSize);
}

void Layer_image_advanced::onCustomMaskActive(bool & _val)
{
    if (_val) {
        Layer_file::onDeactivate();
        ofAddListener(layer_manager->canvasMousePressed , this, &Layer_image_advanced::onCustomMousePressed );
        ofAddListener(layer_manager->canvasMouseDragged , this, &Layer_image_advanced::onCustomMouseDragged );
        ofAddListener(layer_manager->canvasMouseScrolled, this, &Layer_image_advanced::onCustomMouseScrolled);
    }
    else {
        ofRemoveListener(layer_manager->canvasMousePressed , this, &Layer_image_advanced::onCustomMousePressed );
        ofRemoveListener(layer_manager->canvasMouseDragged , this, &Layer_image_advanced::onCustomMouseDragged );
        ofRemoveListener(layer_manager->canvasMouseScrolled, this, &Layer_image_advanced::onCustomMouseScrolled);
        Layer_file::onActivate();
    }

}
