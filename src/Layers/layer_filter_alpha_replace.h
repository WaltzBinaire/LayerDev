#pragma once
#include "Layers\layer_filter_shader.h"

class Layer_filter_alpha_replace : public Layer_filter_shader {
public:
    Layer_filter_alpha_replace(string name, int instance, Layer_Manager * _layer_manager) : Layer_filter_shader(name, instance, _layer_manager) {};

    void handle_file(const string & _path);
    void set_placing() {
        if (imagePaths.size() > 0) {            
            currentPath = imagePaths.end() - 1;
            image.load(*currentPath);
            b_placing = true;
            redraw();
        }
    }

    size_t getNumPaths() const { return imagePaths.size(); }
    glm::vec2 getCurrentImageResolution() const { return glm::vec2(image.getWidth(), image.getHeight()); }

    virtual string getCursorData() const  override { return "s " + ofToString(replacementScale); }

protected:
    virtual void onSetup()  override;
    virtual void onRender(const ofTexture & _baseTex, bool _forced = false) const override;
    virtual void onSetupParams()    override;
    virtual void onReset();
    virtual void onSetupListeners() override;
    virtual void onDrawOverlay()    override;
    virtual void onDrawGui()        override;
    virtual void onResize()         override;


    void setupReplacementFbo();
    virtual void renderReplacmentFbo() const;

    virtual ofTexture & getAlphaTexture() const = 0;

    const static vector<string> get_allowed_exts();

    void onFileDragEvent(ofDragInfo & _fileInfo);
    void onKeyPressed   (ofKeyEventArgs & _args);

    void onMousePressed ( ofMouseEventArgs & args);
    void onMouseScrolled( ofMouseEventArgs & args);
    void onMouseMoved   ( ofMouseEventArgs & args);

    void onHFlipChanged(bool & _val);
    void onLoadFile(bool & _loadFile);

    virtual void setupShader() override;
    virtual void setUniforms(const ofTexture & _baseTex) const override;
    
    ofParameter<bool> p_loadFile;
    ofParameter<bool> p_hFlip;

protected:
    mutable ofFbo   replacementFbo;
    vector<string>::iterator currentPath;
    vector<string> imagePaths;

    mutable ofImage image;

    ofEventListener on_hFlipChanged;
    ofEventListener l_onKeyPressed;

    mutable glm::vec2 replacementPosition;
    mutable float     replacementScale;

    bool b_placing;
    glm::vec2 clickPosition, initPosition;
};


