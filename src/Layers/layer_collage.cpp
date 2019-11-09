#include "Layers\layer_collage.h"
#include "GUI/SingleLayerGui.h"



void Layer_collage::onSetupListeners()
{    
    l_onFileDragged = ofEvents().fileDragEvent.newListener( this, &Layer_collage::onFileDragEvent);
}

//--------------------------------------------------------------
void Layer_collage::onSetup()
{
    collageShader = Shader_lib::get_collage_shader();    
    setupCollageFbo();
    active_patch != nullptr;
}

void Layer_collage::onUpdate()
{
    if (!b_allDrawn || active_patch != nullptr) redraw();
}

//--------------------------------------------------------------
void Layer_collage::onSetupParams()
{    
    p_alphaRange.set("Alpha", glm::vec2(0.8, 0.85), glm::vec2(0.0), glm::vec2(1.0));
    p_alphaRange.addListener(this, &Layer_collage::onAlphaRangeChanged);

    p_loadFolder.set("Load", false);
    p_loadFolder.addListener(this, &Layer_collage::onLoadFolder);

    params.add(
        p_alphaRange,
        p_loadFolder
    );
}

//--------------------------------------------------------------
void Layer_collage::onDraw(bool _forced) const
{
    collageFbo.draw(0, 0);

    if (active_patch != nullptr) {
        if (active_patch->isReady()) {
            ofPushStyle();
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);

            collageShader->begin();
            collageShader->setUniform2f("u_alphaRange", p_alphaRange);

            collageShader->setUniformTexture("tex0", active_patch->getTexture(), 0);
            active_patch->draw();
            collageShader->end();
            ofPopStyle();
        }
    }
}

void Layer_collage::onRender(bool _forced) const
{
    collageFbo.begin();
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

void Layer_collage::onDrawGui()
{
    SingleLayerGui::specialisedDrawGui<Layer_collage>(this); 
}

//--------------------------------------------------------------
void Layer_collage::onReset()
{
    if (imageLoader != nullptr) {
        imageLoader->forceStop();
    }
    active_patch = nullptr;
    patches.clear();
    setupCollageFbo();
}

void Layer_collage::onResize()
{
    setupCollageFbo();
}

void Layer_collage::setupCollageFbo()
{
    collageFbo.allocate(size.x, size.y, GL_RGBA);
    collageFbo.begin();
    ofClear(0.0);
    collageFbo.end();
}


void Layer_collage::onDestroy()
{
    if (imageLoader != nullptr) {
        imageLoader->forceStop();
    }
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
    replace_images(file_path); // Needs to handle more cases!!

}

void Layer_collage::onLoadFolder(bool & _loadFolder)
{
    if (_loadFolder) {
        LayerUtils::loadFolderDialogue(this, &Layer_collage::replace_images);        
    }return;
    _loadFolder = false;

}

void Layer_collage::stopImageLoader()
{
    if (imageLoader != nullptr) {
        imageLoader->forceStop();
    }
    
    imageLoader = make_shared<threadedImageLoader>();
}

void Layer_collage::append_images(const string & _path)
{
    ofDirectory dir(_path);
    if (!dir.exists()) {
        return;
    }

    for (auto ext : LayerUtils::img_exts) dir.allowExt(ext);

    dir.listDir();

    for (auto file : dir) {
        paths.push_back(file.path());
    }
}

string Layer_collage::getCursorData() const {
    if (active_patch != nullptr) {
        return "s " + ofToString(active_patch->getScale());
    }
    else {
        return "";
    }
}

void Layer_collage::loadAllImages()
{
    stopImageLoader();

    for (auto & patch : patches) {
        if (patch->isSetup()) {
            patch->loadImage(imageLoader);
        }

    }
    redraw();
}

void Layer_collage::loadImage(const shared_ptr<CollagePatch>& patch)
{
    if (imageLoader == nullptr) {
        imageLoader = make_shared<threadedImageLoader>();
    }
    patch->loadImage(imageLoader);
}
