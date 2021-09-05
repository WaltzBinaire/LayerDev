#pragma once
#include "ofMain.h"
#include "Layers/layer_base.h"
#include "Utils/shader_base.h"
#include "Utils/LayerUtils.h"

class CollagePatch {
public:
    CollagePatch(string & _path) :
        path(_path),
        center(0.0),
        angle(0.0),
        scale(1.0),
        b_drawn(false),
        b_pending(false),
        b_isSetup(false)
    {
    }

    ~CollagePatch() {
    }

    void loadImage(shared_ptr<threadedImageLoader> imageLoader) {
        image.clear();
        b_pending = true;
        imageLoader->loadFromDisk(image, path);
    }

    void setNewPath(string _path) {
        unload();
        path = _path;
    }

    void unload() {
        image.clear();
        b_pending = false;
    }

    void setDrawn() {
        b_drawn = true;
        b_pending = false;
        unload();
    }

    ofImage & getImageReference()        { return image;  }
    const ofTexture & getTexture() const { return image.getTexture(); }
    

    void draw() {
        auto quad = LayerUtils::UVQuad::getInstance();
        if (!b_isSubsection) {
            ofPushMatrix();
            ofTranslate(getCenter());
            ofRotateRad(getAngle());
            quad.draw(-getSize() * 0.5, getSize());
            ofPopMatrix();
        }
        else {
            float uvW= (size.x / size.y) / (image.getWidth() / image.getHeight());
            glm::vec2 uvX = glm::vec2(0.5 - 0.5 * uvW,  0.5 + 0.5 * uvW );
            quad.draw(getPosition(), getSize(), uvX);
        }
    }

    void setScale(float _scale)       { scale  = _scale;  b_drawn =  false; }
    void setRotation(float _angle)    { angle  = _angle;  b_drawn =  false; }
    void setCenter(glm::vec2 _center) { center = _center; b_drawn =  false; }


    float     getScale()    const { return scale;  }
    glm::vec2 getCenter()   const { return center; }
    float     getAngle()    const { return angle;  }

    bool isReady()    const { 
        return image.isUsingTexture() && image.isAllocated();  
    }

    bool isDrawn()   const { return b_drawn;   }
    bool isPending() const { return b_pending; }
    bool isSetup()  const  { return b_isSetup; }

    void setup(glm::vec2 _center, float _scale, float _angle) {
        b_isSubsection = false;
        b_drawn   =  false;
        b_isSetup =  true;
        center    = _center;
        angle     = _angle;
        scale     = _scale;
    }
    void setupLine(glm::vec2 _center, glm::vec2 _size) {
        b_drawn        =  false;
        b_isSubsection =  true;
        b_isSetup      =  true;
        center         = _center;
        angle          = 0.0;
        size           = _size;
    }

    CollagePatch(const CollagePatch&) = delete;
    CollagePatch& operator=(const CollagePatch&) = delete;

private:

    glm::vec2 getPosition() const { return center - 0.5 * getSize(); }

    glm::vec2 getSize()     const { 
        return glm::vec2(getImageWidth(),getImageHeight()) * scale; 
    }
    
    float getImageWidth()  const  { 
        if (b_isSubsection)  return size.x;
        else return  image.getWidth();
    }
    float getImageHeight() const  {
        if (b_isSubsection)  return size.y;
        else return  image.getHeight();
    }


    string path;
    ofImage image;
    glm::vec2 center;
    float angle;
    float scale;
    glm::vec2 size;
    float offset;

    bool b_drawn;
    bool b_isSetup;
    bool b_pending;
    bool b_isSubsection;
};

class Layer_collage : public Static_base
{
public:

    Layer_collage(string name, int instance, Layer_Manager * _layer_manager)  : Static_base(name, instance, _layer_manager) {};

    void append_images(const string & _path);
    void replace_images(const string & _path) { paths.clear();  append_images(_path); }

    virtual string getCursorData() const  override;
    virtual size_t getNumPaths()   const { return paths.size();  }
    
protected:

    virtual void onSetupListeners() override;
    virtual void onSetup()          override;
    virtual void onUpdate()         override;
    virtual void onSetupParams()    override ;
    virtual void onDraw(bool _forced = false)   const  override ;
    virtual void onDrawOverlay()    override;
    virtual void onRender(bool _forced = false) const  override ;
    virtual void onDrawGui()        override ;
    virtual void onDestroy();
    virtual void onReset()          override;    
    virtual void onResize()         override;

    void setupCollageFbo();

    void loadAllImages();
    void loadImage(const shared_ptr<CollagePatch> & patch);

    void onFileDragEvent(ofDragInfo & _fileInfo);
    void onAlphaRangeChanged(glm::vec2 & _val);
    void onLoadFolder(bool & _loadFolder);

    void stopImageLoader();

    mutable ofFbo collageFbo;

    mutable bool b_allDrawn;

    ofParameter<glm::vec2> p_alphaRange;
    ofParameter<bool>      p_loadFolder;

    vector<shared_ptr<CollagePatch>> patches;
    vector<string>                   paths;
    shared_ptr<CollagePatch>         active_patch;
    mutable shared_ptr<AutoShader>   collageShader;
private:
    mutable shared_ptr<AutoShader>  collageOverlayShader;

    shared_ptr<threadedImageLoader> imageLoader;


};

