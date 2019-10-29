#include "layer_image_advanced.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_image_advanced, Mask Image)

using namespace ofxCv;
using namespace cv;

void Layer_image_advanced::onSetup()
{
    finder.setup("models/haarcascade_frontalface_default.xml");
    finder.setPreset(ObjectFinder::Accurate);

    setupMaskComposeFbo();
    setupCustomMask();
}

void Layer_image_advanced::onDrawGui()
{
    SingleLayerGui::specialisedDrawGui<Layer_image_advanced>(this); 
}

void Layer_image_advanced::onDrawOverlay()
{
    if (isActive("Custom Mask")) {

        float edges[4]{
            brushPosition.x - brushSize * 0.5,
            brushPosition.y - brushSize * 0.5,
            brushPosition.x + brushSize * 0.5,
            brushPosition.y + brushSize * 0.5,
        };

        glm::vec2 points[4]{
            {edges[0], edges[1]},
            {edges[2], edges[1]},
            {edges[2], edges[3]},
            {edges[0], edges[3]}
        };


        ofSetColor(ofColor::grey);
        ofDrawLine(points[0], points[1]);
        ofDrawLine(points[1], points[2]);
        ofDrawLine(points[2], points[3]);
        ofDrawLine(points[3], points[0]);

        string mousePosition_srt = ofToString(brushPosition.x) + ", " + ofToString(brushPosition.y);
        ofDrawBitmapStringHighlight(mousePosition_srt, points[4]);
    }


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

        maskComposeFbo.begin();
        ofClear(0.0);
        maskComposeFbo.end();

        bool isFirst = true;
        for (auto & maskEl : masks) {
            Mask * mask = maskEl.second;
            if (mask->isDrawn()) {                
                QuadMask * quadMask = dynamic_cast<QuadMask *>(mask);
                if (quadMask != nullptr) {
                    quadMask->draw( maskComposeFbo, position, scale, isFirst);                    
                }
                else {
                    mask->draw( maskComposeFbo, isFirst);
                }
                isFirst = false;
            }
        }

        clearMaskFbo();
        maskFbo.begin();
        maskComposeFbo.draw(0, 0);  
        maskFbo.end();
    }
}

void Layer_image_advanced::onResize()
{
    customMask.resize(size);
    setupMaskComposeFbo();
}

void Layer_image_advanced::setupMaskComposeFbo()
{
    maskComposeFbo.allocate(size.x, size.y, GL_RGBA); 
    maskComposeFbo.clearAll();
}

void Layer_image_advanced::setupFaceMask()
{
    finder.update(img);
    faceMask.setup(size.x, size.y);
    faceMask.setupQuad(img.getWidth(), img.getHeight());

    faceMask.beginQuad();
    for (int i = 0; i < finder.size(); i++) {
        auto rect = finder.getObject(i);
        ofSetColor(ofColor::white);
        ofDrawRectangle(rect);
    }

    faceMask.endQuad();

    masks.insert_or_assign("Face Mask", &faceMask);

}

void Layer_image_advanced::setupCustomMask()
{
    customMask.setup(size.x, size.y);
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

    bodyMask.setup(size.x, size.y);
    bodyMask.setupQuad(img.getWidth(), img.getHeight());

    bodyMask.beginQuad();
    bodyMaskImg.draw(0, 0, img.getWidth(), img.getHeight());
    bodyMask.endQuad();

    masks.insert_or_assign("Body Mask", &bodyMask);
    setEnabled("Body Mask", true);
}

void Layer_image_advanced::onCustomMousePressed(ofMouseEventArgs & _args)
{
    drawBrush(_args);
}

void Layer_image_advanced::drawBrush(ofMouseEventArgs & _args)
{
    ofPushStyle();
    ofLogNotice() << _args.button;

    brushPosition = glm::vec2(_args.x, _args.y);

    brushPosition = brushSize * glm::floor(brushPosition / brushSize);

    if (_args.button == 0) ofSetColor(ofColor::black);
    else                   ofSetColor(ofColor::white);

    customMask.begin();
    ofDrawRectangle(
        brushPosition.x - brushSize * 0.5,
        brushPosition.y - brushSize * 0.5,
        brushSize,
        brushSize
    );

    customMask.end();
    redraw();
    ofPopStyle();
}

void Layer_image_advanced::onCustomMouseDragged(ofMouseEventArgs & _args)
{
    drawBrush(_args);
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
