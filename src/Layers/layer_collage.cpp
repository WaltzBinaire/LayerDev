#include "Layers\layer_collage.h"

const vector<string> Layer_collage::allowed_extensions {
    "png",
    "jpg",
    "gif"
};

//--------------------------------------------------------------
void Layer_collage::onSetup()
{
    collageShader = Shader_lib::get_collage_shader();
    setupQuad();
}

//--------------------------------------------------------------
void Layer_collage::onSetupParams()
{    
    p_alphaRange.set("Alpha", glm::vec2(0.5, 0.8), glm::vec2(0.0), glm::vec2(1.0));
    p_alphaRange.addListener(this, &Layer_collage::onAlphaRangeChanged);
    params.add(p_alphaRange);
}

//--------------------------------------------------------------
void Layer_collage::onDraw() const
{
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    collageShader->begin();
    collageShader->setUniform2f("u_alphaRange", p_alphaRange);
    for(const auto  & colImage : images) {
        
        collageShader->setUniformTexture("tex0", colImage.image.getTexture(), 0);
        setQuad(colImage);
        drawQuad.draw();
    }

    collageShader->end();

    ofPopStyle();

    if (mode == Mode::EDITING) {
        ofSetColor(ofColor::gray);
        ofDrawBitmapString("EDITING", glm::vec2(50, 50));
    }

}

//--------------------------------------------------------------
void Layer_collage::onUpdate()
{
    switch (mode) {
    case Mode::EDITING:
        redraw();
        break;
    case Mode::VIEWING:
    default:
        break;
    }

}

//--------------------------------------------------------------
void Layer_collage::onReset()
{
    images.clear();
}


//--------------------------------------------------------------
void Layer_collage::setMode(Mode _mode)
{
    switch (_mode) {
    case Mode::EDITING:
        onModeEditing();
        redraw();
        break;
    case Mode::VIEWING:
        onModeViewing();
        break;
    default:
        break;
    }

    mode = _mode;
}

void Layer_collage::onAlphaRangeChanged(glm::vec2 & _val)
{
    if (_val.x > _val.y) _val.x = _val.y;
    redraw();
}



//--------------------------------------------------------------
void Layer_collage::setupQuad()
{
    drawQuad.addVertex(glm::vec3(0));
    drawQuad.addVertex(glm::vec3(0));
    drawQuad.addVertex(glm::vec3(0));
    drawQuad.addVertex(glm::vec3(0));

    drawQuad.addTexCoord(glm::vec2(0.0, 0.0));
    drawQuad.addTexCoord(glm::vec2(1.0, 0.0));
    drawQuad.addTexCoord(glm::vec2(1.0, 1.0));
    drawQuad.addTexCoord(glm::vec2(0.0, 1.0));

    drawQuad.addIndex(0);
    drawQuad.addIndex(1);
    drawQuad.addIndex(2);

    drawQuad.addIndex(2);
    drawQuad.addIndex(3);
    drawQuad.addIndex(0);
}

//--------------------------------------------------------------
void Layer_collage::setQuad(const CollageImage &colImage) const
{
    glm::vec2 imageSize = glm::vec2(colImage.image.getWidth(), colImage.image.getHeight());
    glm::vec2 size  = imageSize * colImage.scale;
    glm::vec2 pos   = colImage.center - size * 0.5;
    
    const ofImage & image = colImage.image;

    glm::vec3 pos_00 = glm::vec3(pos.x, pos.y, 0);
    glm::vec3 pos_10 = glm::vec3(pos.x + size.x, pos.y, 0);
    glm::vec3 pos_11 = glm::vec3(pos.x + size.x, pos.y + size.y, 0);
    glm::vec3 pos_01 = glm::vec3(pos.x, pos.y + size.y, 0);


    drawQuad.setVertex(0, pos_00);
    drawQuad.setVertex(1, pos_10);
    drawQuad.setVertex(2, pos_11);
    drawQuad.setVertex(3, pos_01);
}

//--------------------------------------------------------------
bool Layer_collage::extensionValid(const ofFile _file)
{
    return std::find(allowed_extensions.begin(), allowed_extensions.end(), _file.getExtension()) != allowed_extensions.end();
}
