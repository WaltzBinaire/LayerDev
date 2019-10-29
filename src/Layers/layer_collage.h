#pragma once
#include "ofMain.h"
#include "Layers/layer_base.h"
#include "Utils/shader_base.h"
#include "Utils/LayerUtils.h"


class CollagePatch {
public:
    CollagePatch() :
        center(0.0),
        angle(0.0),
        scale(1.0),
        b_positionSet(false)
    {
    }

    ~CollagePatch() {}

    ofImage & getImageReference() { return image;  }
    const ofTexture & getTexture() const { return image.getTexture(); }

    float getImageWidth() const { return  image.getWidth();}
    float getImageHeight() const { return image.getHeight();}

    float getScale()  const { return scale; }
    glm::vec2 getCenter() const { return center; }
    float getAngle() const  { return angle; }

    bool isSetup()    const { 
        return image.isUsingTexture() && b_positionSet;  
    }
    bool needsSetup() const {
        return !b_positionSet && image.isUsingTexture(); 
    }

    void setup(glm::vec2 _center, float _scale, float _angle) {
        center  = _center;
        angle   = _angle;
        scale   = _scale;
        b_positionSet = true;
    }

    CollagePatch(const CollagePatch&) = delete;
    CollagePatch& operator=(const CollagePatch&) = delete;

private:
    ofImage image;
    glm::vec2 center;
    float angle;
    float scale;
    bool b_positionSet;
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

    virtual void onSetupListeners() override ;
    virtual void onSetup()          override ;
    virtual void onSetupParams()    override ;
    virtual void onDraw()  const    override ;
    virtual void onDrawGui()        override ;
    virtual void onUpdate()         override ;
    virtual void onReset()          override ;    

    virtual void setupPatch(CollagePatch & _patch, int _idx) = 0;

    void updatePatches();

    void onFileDragEvent(ofDragInfo & _fileInfo);
    void onLoadFolder(bool & _loadFolder);


    void setupQuad();
    void setQuad(const CollagePatch &colImage) const;

    void setMode(Mode _mode);

    virtual void onModeViewing() {};
    virtual void onModeEditing() {};

    Mode mode;

    ofParameter<glm::vec2> p_alphaRange;
    ofParameter<bool>      p_loadFolder;

    vector<string> image_paths;
    vector<shared_ptr<CollagePatch>> images;

    struct {
        int nSetup;
        int nNeedsSetup;
        int nLoading;
        int nTotal;

        void reset() {
            nSetup      = 0;
            nNeedsSetup = 0;
            nLoading    = 0;
            nTotal      = 0;
        }
    } patchInfo;

private:


    vector<CollagePatch>::iterator active_image;

    mutable shared_ptr<AutoShader> collageShader;
    mutable ofMesh   drawQuad;



};

