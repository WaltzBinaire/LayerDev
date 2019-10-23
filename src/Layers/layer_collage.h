#pragma once
#include "ofMain.h"
#include "Layers/layer_base.h"
#include "Utils/shader_base.h"
#include "Utils/LayerUtils.h"


struct CollageImage {
    ofImage image;
    glm::vec2 center;
    float angle;
    float scale;

    CollageImage(ofImage & _image, glm::vec2 _center) :
        image(_image),
        center(_center),
        scale(1.0),
        angle(0.0)
    {}
};

class Layer_collage : public Static_base
{
public:

    Layer_collage(string name, int instance, Layer_Manager * _layer_manager)  : Static_base(name, instance, _layer_manager) {};

    void onAlphaRangeChanged(glm::vec2 & _val);
    void populate_images(const string & _path);
protected:

    enum class Mode {
        VIEWING = 0,
        EDITING
    };

    virtual void onActivate()    override ;
    virtual void onDeactivate()  override ;
    virtual void onSetup()       override ;
    virtual void onSetupParams() override ;
    virtual void onDraw()  const override ;
    virtual void onDrawGui()     override ;
    virtual void onUpdate()      override ;
    virtual void onReset()       override ;    

    void onFileDragEvent(ofDragInfo & _fileInfo);
    void onLoadFolder(bool & _loadFolder);


    void setupQuad();
    void setQuad(const CollageImage &colImage) const;

    void setMode(Mode _mode);

    virtual void onModeViewing() {};
    virtual void onModeEditing() {};

    ofParameter<glm::vec2> p_alphaRange;
    ofParameter<bool>      p_loadFolder;

    vector<string> image_paths;

    Mode mode;
    
    vector<CollageImage> images;
    vector<CollageImage>::iterator active_image;

    shared_ptr<AutoShader> collageShader;
    mutable ofMesh   drawQuad;



};

