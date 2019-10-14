#include "Layers\layer_filter_alpha_replace.h"
#include "Utils\LayerUtils.h"


void Layer_filter_alpha_replace::onSetupParams()
{    
    p_loadFile.set("Load", false);
    p_loadFile.addListener(this, &Layer_filter_alpha_replace::onLoadFile);

    params.add(
        p_loadFile
    );
}

void Layer_filter_alpha_replace::setupShader()
{
    shader = Shader_lib::get_alpha_shader();
}

void Layer_filter_alpha_replace::onActivate()
{    
    ofAddListener(ofEvents().fileDragEvent, this, &Layer_filter_alpha_replace::onFileDragEvent);
}   

void Layer_filter_alpha_replace::onDeactivate()
{
    ofRemoveListener(ofEvents().fileDragEvent, this, &Layer_filter_alpha_replace::onFileDragEvent);
}

void Layer_filter_alpha_replace::setUniforms(const ofTexture & _baseTex) const
{
    shader->setUniformTexture("u_imageTex"      , _baseTex          , 0);
    shader->setUniformTexture("u_alphaTex"      , getAlphaTexture() , 1);

    if(replacementTexture.isAllocated())
        shader->setUniformTexture("u_replacementTex", replacementTexture, 2);
}


const vector<string> Layer_filter_alpha_replace::get_allowed_exts()
{
    const vector<string> allowed_exts {
            "jpg",
            "JPG",
            "jpeg",
            "JPEG",
            "png",
            "PNG",
            "gif",
            "GIF"
    };
    return allowed_exts;
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

void Layer_filter_alpha_replace::onLoadFile(bool & _loadFile)
{
    if (_loadFile) {
        LayerUtils::loadFileDialogue(get_allowed_exts(), this, &Layer_filter_alpha_replace::handle_file);
    }
    _loadFile = false;
}

void Layer_filter_alpha_replace::handle_file(const string & _path)
{
    ofImage img;
    img.setUseTexture(false);
    img.load(_path);
    replacementTexture.loadData(img);
    replacementTexture.setTextureWrap(GL_REPEAT, GL_REPEAT );  
}
