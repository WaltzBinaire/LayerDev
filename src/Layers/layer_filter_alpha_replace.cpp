#include "Layers\layer_filter_alpha_replace.h"
#include "Utils\LayerUtils.h"
#include "GUI/SingleLayerGui.h"


void Layer_filter_alpha_replace::onSetup()
{
    Layer_filter_shader::onSetup();

    replacementScale = 1.0f;
    replacementPosition = size * 0.5;
    setupReplacementFbo();

    currentImage = images.end();
}

void Layer_filter_alpha_replace::onRender(const ofTexture & _baseTex) const
{
    renderReplacmentFbo();
}

void Layer_filter_alpha_replace::onSetupParams()
{    
    p_loadFile.set("Load", false);
    p_loadFile.addListener(this, &Layer_filter_alpha_replace::onLoadFile);

    params.add(
        p_loadFile
    );
}

void Layer_filter_alpha_replace::onReset()
{
    images.clear();
    currentImage = images.end();
}

void Layer_filter_alpha_replace::setupShader()
{
    shader = Shader_lib::get_alpha_shader();
}

void Layer_filter_alpha_replace::onSetupListeners()
{    
    l_onFileDragged   = ofEvents().fileDragEvent.newListener          ( this, &Layer_filter_alpha_replace::onFileDragEvent);
    l_onMousePressed  = layer_manager->canvasMousePressed.newListener ( this, &Layer_filter_alpha_replace::onMousePressed);
    l_onMouseScrolled = layer_manager->canvasMouseScrolled.newListener( this, &Layer_filter_alpha_replace::onMouseScrolled);
    l_onMouseMoved    = layer_manager->canvasMouseMoved.newListener   ( this, &Layer_filter_alpha_replace::onMouseMoved);

}

void Layer_filter_alpha_replace::onDrawGui()
{
    SingleLayerGui::specialisedDrawGui<Layer_filter_alpha_replace>(this); 
}

void Layer_filter_alpha_replace::onResize()
{
    setupReplacementFbo();
}

void Layer_filter_alpha_replace::setupReplacementFbo()
{
    replacementFbo.allocate(size.x, size.y, GL_RGBA);
}

void Layer_filter_alpha_replace::renderReplacmentFbo() const
{
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


void Layer_filter_alpha_replace::setUniforms(const ofTexture & _baseTex) const
{
    shader->setUniformTexture("u_imageTex"      , _baseTex                    , 0);
    shader->setUniformTexture("u_alphaTex"      , getAlphaTexture()           , 1);
    shader->setUniformTexture("u_replacementTex", replacementFbo.getTexture() , 2);
}

const vector<string> Layer_filter_alpha_replace::get_allowed_exts()
{  
    return LayerUtils::img_exts;
}

void Layer_filter_alpha_replace::onFileDragEvent(ofDragInfo & _fileInfo)
{
    if (_fileInfo.files.size() > 1) ofLogWarning() << "Can't add multiple images.";

    string file_path = _fileInfo.files[0];
    ofFile file(file_path);

    if (!file.exists()) {
        ofLogWarning(name) << "Cannot load: " << file.path();
        return;
    }

    const vector<string> & allowed_exts = get_allowed_exts();

    if (std::find(allowed_exts.begin(), allowed_exts.end(), file.getExtension()) == allowed_exts.end()) {
        ofLogWarning(name) << "Cannot load: " << file.path() << "\nWrong type";
        return;
    }
    ofLogVerbose(name) << "Handling " << file.path();
    handle_file(file.path());
}

void Layer_filter_alpha_replace::onMousePressed(ofMouseEventArgs & args)
{
    b_placing = !b_placing;
    clickPosition = glm::vec2(args.x, args.y);
    initPosition = replacementPosition;
}

void Layer_filter_alpha_replace::onMouseScrolled(ofMouseEventArgs & args)
{
    if (b_placing) {
        replacementScale += args.scrollY * 0.05;
        replacementScale = max(0.1f, replacementScale);
        redraw();
    }
    else {
        switch ((int)args.scrollY) {
        case 1:
            currentImage++;
            if (currentImage >= images.end())   currentImage = images.begin();
            break;
        case -1:
            if (currentImage == images.begin()) {
                currentImage = images.end() - 1;
            }
            else {
                currentImage--;
            }
            break;
        }

        redraw();
    }
}

void Layer_filter_alpha_replace::onMouseMoved(ofMouseEventArgs & args)
{
    if (b_placing) {
        replacementPosition = initPosition + ( glm::vec2(args.x, args.y) - clickPosition);
        redraw();
    }
}

void Layer_filter_alpha_replace::onLoadFile(bool & _loadFile)
{
    if (_loadFile) {
        LayerUtils::loadFileDialogue(get_allowed_exts(), this, &Layer_filter_alpha_replace::handle_file);
    }
    _loadFile = false;
}

void Layer_filter_alpha_replace::handle_file(const string & _path)
{
    images.emplace_back(_path);
    currentImage = images.end() - 1;
    redraw();
}
