#include "Layers\layer_alpha_replace_face.h"

REGISTER_TYPE(Layer_alpha_replace_face)

void Layer_alpha_replace_face::onSetup()
{
    Layer_filter_shader::onSetup();

    tracker.setup();
}

void Layer_alpha_replace_face::onDestroy()
{
    ofLogNotice(name) << "Stopping tracker";
    tracker.stop();
}

void Layer_alpha_replace_face::onDraw(const ofTexture & _baseTex) const
{
    updateFace(_baseTex);
    Layer_filter_shader::onDraw(_baseTex);
}

void Layer_alpha_replace_face::onSetupParams()
{
    Layer_filter_alpha_replace::onSetupParams();

}

void Layer_alpha_replace_face::setupShader()
{
    //shader = Shader_lib::get_alpha_shader();
    shader = Shader_lib::get_alpha_multi_shader();

    face_shader = Shader_lib::get_face_shader();
    l_onFaceShaderLoad = face_shader->onLoad.newListener([&](bool &) {return  redraw(); });

}

void Layer_alpha_replace_face::updateFace(const ofTexture & _baseTex) const
{
    if (!faceFbo.isAllocated()) faceFbo.allocate((int)_baseTex.getWidth(), (int)_baseTex.getHeight(), GL_RGBA);
    
    if (faceFbo.getWidth() != _baseTex.getWidth() || faceFbo.getHeight() != _baseTex.getHeight()) 
        faceFbo.allocate((int)_baseTex.getWidth(), (int)_baseTex.getHeight(), GL_RGBA);

    _baseTex.readToPixels(pixels);

    if (pixels.isAllocated()) {
        tracker.update(pixels);

        faceFbo.begin();
        ofClear(0, 0);
        face_shader->begin();

        ofPushStyle();
        for (auto instance : tracker.getInstances()) {
            ofMesh& faceMesh = instance.getLandmarks().getImageMesh();
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

        
        
        ofPopStyle();

        face_shader->end();
        faceFbo.end();
    }


}
