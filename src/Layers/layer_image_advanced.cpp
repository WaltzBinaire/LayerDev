#include "layer_image_advanced.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_image_advanced, Mask Image)

using namespace ofxCv;
using namespace cv;

void Layer_image_advanced::onSetup()
{

#ifdef NDEBUG
    finder.setup(); 
    finder.setThreaded(false);
#else
    finder.setup("models/haarcascade_frontalface_default.xml");
    finder.setPreset(ObjectFinder::Accurate);
#endif // !NDEBUG


    setupMaskComposeFbo();
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

        maskComposeFbo.begin();
        ofClear(255);
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
    setupMaskComposeFbo();
}

void Layer_image_advanced::onDestroy()
{
#ifdef NDEBUG
    finder.stop();
#endif // !NDEBUG
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
    ofBackground(0.0);
#ifdef NDEBUG
        for (auto instance : finder.getInstances()) {
            ofMesh& faceMesh = instance.getLandmarks().getImageMesh();
            ofSetColor(ofColor::white);
            faceMesh.draw();
        }
#else
    for (int i = 0; i < finder.size(); i++) {
        auto rect = finder.getObject(i);
        ofSetColor(ofColor::white);
        ofDrawRectangle(rect);
    }
#endif // !NDEBUG

    faceMask.endQuad();

    masks.insert_or_assign("Face Mask", &faceMask);

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
}
