#pragma once
#include "Layers\layer_file_image.h"
#include "ofxCv.h"

class Mask {
public:
    enum class ADD_MODE {ADD, SUBTRACT, DISABLE };

    Mask() :
        addMode(ADD_MODE::DISABLE),
        b_isActive(false)
    { }

    virtual void setup(int width, int height) { 
        fbo.allocate(width, height, GL_RGBA); 
        fbo.begin();
        ofClear(0);
        ofBackground(0);
        fbo.end();
    }
    void begin() { fbo.begin(); }
    void end()   { fbo.end();   }

    ADD_MODE getAddMode() const { return addMode; }

    bool isDrawn()       const { 
        return fbo.isAllocated() && addMode != ADD_MODE::DISABLE; 
    }

    void setMode(ADD_MODE _mode) { addMode = _mode;    }
    void setActive(bool _active) { 
        b_isActive = _active; 
    }
    bool isActive()  { return b_isActive;   }


protected:
    ofFbo fbo;
    ADD_MODE addMode;
    bool b_isActive;
};
class QuadMask : public Mask {
public:
    void draw(glm::vec2 _position, float scale) const { 
        fbo.draw(   
            _position.x - 0.5 * scale * fbo.getWidth(),
            _position.y - 0.5 * scale * fbo.getHeight(),
            scale * fbo.getWidth(),
            scale * fbo.getHeight()        
        ); 
    
    }

};
class StretchMask : public Mask {    
public:
    void draw() const { fbo.draw(0, 0); }
    void resize(glm::vec2 _size) {};
};


class Layer_image_advanced : public Layer_file_image
{
public:

    Layer_image_advanced(string name, int instance, Layer_Manager * _layer_manager) : Layer_file_image(name, instance, _layer_manager) {};

    void loadBodyMask(const string & path);
    virtual void handle_file(const string & _path) override;

    void setMaskMode(const string & _mask, Mask::ADD_MODE _mode) {
        auto mask = masks.find(_mask);
        if (mask != masks.end()) {
            mask->second->setMode(_mode);
            redraw();
        }
    }
    void setActive(const string & _mask, bool _active) {
        for (auto & mask : masks) {
            if (mask.first == _mask) {
                mask.second->setActive(_active);

                if (_mask == "Custom Mask") onCustomMaskActive(_active);
            }
            else {
                mask.second->setActive(false);
            }
        }
    }
    
    bool isActive(const string & _mask) {
        auto mask = masks.find(_mask);
        if (mask != masks.end()) {
            return (mask->second->isActive());
        }
        return false;
    }

    Mask::ADD_MODE  getMaskMode(const string & _mask) { 
        auto mask = masks.find(_mask);
        if (mask != masks.end()) {
            return (mask->second->getAddMode());
        }
        return Mask::ADD_MODE::DISABLE;
    }

    vector<string> getMasks() { 
        vector<string> maskKeys;
        maskKeys.reserve(masks.size());
        for (auto const& maskEl : masks) maskKeys.push_back(maskEl.first);
        return maskKeys;  
    }

protected:
    virtual void onSetup()   override;
    virtual void onDrawGui() override;
    virtual void onRender()  const override;
    virtual void onResize()  override;

    virtual void setupFaceMask();
    virtual void setupCustomMask();
    virtual void setupBodyMask(const string & _path);

    void onCustomMousePressed(ofMouseEventArgs & _args);
    void drawBrush(ofMouseEventArgs & _args);
    void onCustomMouseDragged(ofMouseEventArgs & _args);
    void onCustomMouseScrolled(ofMouseEventArgs & _args);

    void onCustomMaskActive(bool & _val);

    map<string, Mask*> masks;

    QuadMask    bodyMask;
    QuadMask    faceMask;
    StretchMask customMask;

private:
    ofxCv::ObjectFinder finder;

    // Custom Draw
    float brushSize = 50;
    const float maxBrushSize = 200;
    const float minBrushSize = 10;
    glm::vec2 brushPosition;
};

