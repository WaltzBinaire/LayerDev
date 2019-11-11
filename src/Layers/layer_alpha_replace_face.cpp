#include "Layers\layer_alpha_replace_face.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_alpha_replace_face, Face Replace)

void Layer_alpha_replace_face::onSetup()
{
    Layer_filter_alpha_replace::onSetup();

    setupDetectionFbo();
    b_faceMaskInitialised = false;

#ifdef NDEBUG
    tracker.setup();
    tracker.setThreaded(false);
#endif // !NDEBUG    
    setupFaceFbo();
}

void Layer_alpha_replace_face::onSetupParams()
{
    Layer_filter_alpha_replace::onSetupParams();


    p_useMask.set("Mask Face", true);
    p_lock.set   ("Lock"     , true);

    params.add(
        p_useMask,
        p_lock
    );
}

void Layer_alpha_replace_face::onDestroy()
{
#ifdef NDEBUG
    tracker.stop();
#endif // !NDEBUG  
}
void Layer_alpha_replace_face::onRender(const ofTexture & _baseTex, bool _forced) const
{
    if ((p_useMask && _forced && !p_lock) || !b_faceMaskInitialised ) {
        updateFace(_baseTex);
    }

   renderReplacmentFbo();
}

void Layer_alpha_replace_face::onDraw(const ofTexture & _baseTex, bool _forced) const
{
    if (p_useMask) {
        Layer_filter_alpha_replace::onDraw(_baseTex);
    }
    else {
#ifdef NDEBUG
        if (image.isAllocated())
            _baseTex.draw(0, 0);
            replacementFbo.draw(0, 0);
#endif
    }

}

void Layer_alpha_replace_face::onDrawGui()
{
    Layer_filter_alpha_replace::onDrawGui();
    SingleLayerGui::specialisedDrawGui<Layer_alpha_replace_face>(this); 
}

void Layer_alpha_replace_face::onResize()
{
    Layer_filter_alpha_replace::onResize();
    setupFaceFbo();

    setupDetectionFbo();
}

void Layer_alpha_replace_face::setupDetectionFbo()
{
    detectionScale = getDetectionTextureScale();
    detectionFbo.allocate(detectionScale * size.x, detectionScale * size.y, GL_RGBA);
    pixels.allocate(detectionScale * size.x, detectionScale * size.y, GL_RGBA);
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
    replacementPosition = glm::vec2(
        ofClamp(
            replacementPosition.x, 
            faceRect.getCenter().x - 0.5 * faceRect.getWidth(), 
            faceRect.getCenter().x + 0.5 * faceRect.getWidth()
        ),
        ofClamp(
            replacementPosition.y, 
            faceRect.getCenter().y - 0.5 * faceRect.getHeight(), 
            faceRect.getCenter().y + 0.5 * faceRect.getHeight()
        )
    );  
#endif

    replacementFbo.begin();
    ofClear(0.0, 0.0);
    if (image.isAllocated()) {
        image.draw(
            replacementPosition.x - replacementScale * 0.5 * image.getWidth(),
            replacementPosition.y - replacementScale * 0.5 * image.getHeight(),
            replacementScale * image.getWidth(),
            replacementScale * image.getHeight()
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
    ofClear(0.0, 0.0);
    faceFbo.end();
}

void Layer_alpha_replace_face::updateFace(const ofTexture & _baseTex) const
{
    detectionFbo.begin();
    _baseTex.draw(0, 0, detectionFbo.getWidth(), detectionFbo.getHeight());
    detectionFbo.end();
    detectionFbo.readToPixels(pixels);

    if (pixels.isAllocated()) {

        faceFbo.begin();
        ofClear(0, 0);
        face_shader->begin();
        face_shader->setUniform1f("u_scale", detectionScale);

        ofPushStyle();

#ifdef NDEBUG
        tracker.stop();
        tracker.update(pixels);

        for (auto instance : tracker.getInstances()) {
            ofMesh& faceMesh = instance.getLandmarks().getImageMesh();

            faceMesh.draw();

            //-----------------------------------------
            faceRect = instance.getBoundingBox();
            faceRect.x      /= detectionScale;
            faceRect.y      /= detectionScale;
            faceRect.width  /= detectionScale;
            faceRect.height /= detectionScale;

            b_faceMaskInitialised = true;

            if (p_lock) {
                tracker.stop();
            }
        }
#endif // !NDEBUG    
        ofPopStyle();

        face_shader->end();
        faceFbo.end();
    }

}

float Layer_alpha_replace_face::getDetectionTextureScale() {
    if (size.x > size.y) {
        float _size = max(size.x, MAX_DETECTION_SIZE);
        return _size / size.x;
    }
    else {
        float _size = max(size.y, MAX_DETECTION_SIZE);
        return _size / size.y;
    }
}

