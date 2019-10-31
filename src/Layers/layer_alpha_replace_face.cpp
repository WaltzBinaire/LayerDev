#include "Layers\layer_alpha_replace_face.h"

REGISTER_TYPE(Layer_alpha_replace_face, Face Replace)

void Layer_alpha_replace_face::onSetup()
{
    Layer_filter_alpha_replace::onSetup();

#ifdef NDEBUG
    tracker.setup();
    tracker.setThreaded(false);
#endif // !NDEBUG    
    setupFaceFbo();
}

void Layer_alpha_replace_face::onDestroy()
{
#ifdef NDEBUG
    //tracker.stop();
#endif // !NDEBUG  
}

void Layer_alpha_replace_face::onResize()
{
    Layer_filter_alpha_replace::onResize();
    setupFaceFbo();
}

void Layer_alpha_replace_face::onRender(const ofTexture & _baseTex) const
{
   renderReplacmentFbo();
   updateFace(_baseTex);
}

void Layer_alpha_replace_face::setupShader()
{
    shader = Shader_lib::get_alpha_multi_shader();

    face_shader = Shader_lib::get_face_shader();
    l_onFaceShaderLoad = face_shader->onLoad.newListener([&](bool &) {return  redraw(); });

}

void Layer_alpha_replace_face::renderReplacmentFbo() const
{
#ifdef NDEBUG
    replacementPosition = glm::vec2(faceRect.getCenter().x, faceRect.getCenter().y);  

#endif // !NDEBUG 
    replacementFbo.begin();
    ofClear(0.0);
    if (currentImage != images.end()) {
        currentImage->draw(
            replacementPosition.x - replacementScale * 0.5 * currentImage->getWidth(),
            replacementPosition.y - replacementScale * 0.5 * currentImage->getHeight(),
            replacementScale * currentImage->getWidth(),
            replacementScale * currentImage->getHeight()
            );
    }
    else {
        ofBackground(ofColor::cyan);
    }

    replacementFbo.end();
}


void Layer_alpha_replace_face::setupFaceFbo()
{
    faceFbo.allocate(size.x, size.y, GL_RGBA);
    faceFbo.begin();
    ofClear(0.0);
    faceFbo.end();
}

void Layer_alpha_replace_face::updateFace(const ofTexture & _baseTex) const
{
    _baseTex.readToPixels(pixels);

    if (pixels.isAllocated()) {

        faceFbo.begin();
        ofClear(0, 0);
        face_shader->begin();

        ofPushStyle();

#ifdef NDEBUG
        tracker.update(pixels);

        for (auto instance : tracker.getInstances()) {
            ofMesh& faceMesh = instance.getLandmarks().getImageMesh();

            faceRect = instance.getBoundingBox();

            faceMesh.enableColors();
            vector<ofFloatColor> colors;
            ofFloatColor col = ofFloatColor(ofRandom(0.0, 1.0), 1.0);
            colors.resize(faceMesh.getNumVertices());
            for (int i = 0; i < colors.size(); i+=3) {
                
                if(ofRandomf() < 0.1) ofFloatColor col = ofFloatColor(ofRandom(0.0, 1.0), 1.0);

                colors[i + 0] = col;
                colors[i + 1] = col;
                colors[i + 2] = col;
            }

            faceMesh.addColors(colors);

            faceMesh.draw();
        }
#endif // !NDEBUG    
        ofPopStyle();

        face_shader->end();
        faceFbo.end();
    }


}

