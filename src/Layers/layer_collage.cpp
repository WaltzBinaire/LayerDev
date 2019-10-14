#include "Layers\layer_collage.h"
#include "GUI/SingleLayerGui.h"

void Layer_collage::onActivate()
{    
    ofAddListener(ofEvents().fileDragEvent, this, &Layer_collage::onFileDragEvent);
}

void Layer_collage::onDeactivate()
{
    ofRemoveListener(ofEvents().fileDragEvent, this, &Layer_collage::onFileDragEvent);
}


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

    p_loadFolder.set("Load", false);
    p_loadFolder.addListener(this, &Layer_collage::onLoadFolder);

    params.add(
        p_alphaRange,
        p_loadFolder
    );
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
}

void Layer_collage::onDrawGui()
{
    SingleLayerGui::specialisedDrawGui<Layer_collage>(this); 
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
    image_paths.clear();
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

void Layer_collage::onFileDragEvent(ofDragInfo & _fileInfo)
{
    if (_fileInfo.files.size() > 1) ofLogWarning(__FUNCTION__) << "Can't add multiple images.";
    string file_path = _fileInfo.files[0];
    populate_images(file_path); // Needs to handle more cases!!

}

void Layer_collage::onLoadFolder(bool & _loadFolder)
{
    if (_loadFolder) {
        LayerUtils::loadFolderDialogue(this, &Layer_collage::populate_images);        
    }return;
    _loadFolder = false;

}

void Layer_collage::populate_images(const string & _path)
{
    ofDirectory dir(_path);
    if (!dir.exists()) {
        return;
    }

    for (auto ext : LayerUtils::img_exts) dir.allowExt(ext);

    dir.listDir();

    image_paths.clear();
    for (auto file : dir) {
        image_paths.push_back(file.path());
    }
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
