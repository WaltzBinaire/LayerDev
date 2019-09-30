#pragma once
#include "layer_filter_shader.h"

class Layer_filter_alpha_replace : public Layer_filter_shader {
public:
    Layer_filter_alpha_replace(string name, Layer_Manager * _layer_manager) : Layer_filter_shader(name, _layer_manager) {};

protected:

    virtual void onSetupParams() override;
    virtual void onActivate()    override ;
    virtual void onDeactivate()  override ;

    virtual ofTexture & getAlphaTexture() const = 0;

    const static vector<string> get_allowed_exts();
    void onFileDragEvent(ofDragInfo & _fileInfo);
    void onLoadFile(bool & _loadFile);
    void handle_file(const string & _path);

    virtual void setupShader() override;
    virtual void setUniforms(const ofTexture & _baseTex) const override;
    
    ofParameter<bool> p_invert;
    ofParameter<bool> p_loadFile;

private:

    ofTexture replacementTexture;
};


