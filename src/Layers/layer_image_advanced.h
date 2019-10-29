#pragma once
#include "Layers\layer_file_image.h"
#include "Utils\LayerUtils.h"
#include "Utils\shader_base.h"
#include "ofxCv.h"

class Mask {
public:
    enum class ADD_MODE {
        EQUALS = 0,
        AND    = 1, 
        OR     = 2, 
        XOR    = 3
    };

    Mask() :
        addMode(ADD_MODE::AND),
        b_isActive(false),
        b_isEnabled(false)
    { 
        shader = Shader_lib::get_mask_compose_shader();
    }

    virtual void setup(int width, int height) { 
        maskFbo.allocate(width, height, GL_RGBA); 
        maskFbo.begin();
        ofClear(0);
        ofBackground(255);
        maskFbo.end();
    }

    virtual void resize(glm::vec2 _size) {
        setup(_size.x, _size.y);
    }

    virtual void draw(pingPongFbo & _fbo, bool equals = false) const {
        _fbo.swap();
        _fbo.begin();
        shader->begin();
        setShader(_fbo);
        LayerUtils::UVQuad::getInstance().draw(0, 0, maskFbo.getWidth(), maskFbo.getHeight()); 
        shader->end();
        _fbo.end();
    }

    void begin() const { maskFbo.begin(); }
    void end()   const { maskFbo.end();   }

    ADD_MODE getAddMode() const { return addMode; }

    bool isDrawn()       const { 
        return maskFbo.isAllocated() && b_isEnabled; 
    }

    void setMode(ADD_MODE _mode)   { addMode     = _mode;       }
    void setEnabled(bool _enabled) { b_isEnabled = _enabled;    }
    void setActive(bool _active)   { b_isActive = _active;      }

    bool isActive()  const { return b_isActive;   }
    bool isEnabled() const { return b_isEnabled;  }


protected:

    void setShader(pingPongFbo & _fbo, bool equals = false) const
    {
        shader->setUniformTexture("u_baseTex", _fbo.getBackTexture(), 0);
        shader->setUniformTexture("u_maskTex", maskFbo.getTexture(), 1);

        if (equals) {
            shader->setUniform1i("u_mode", (int)ADD_MODE::EQUALS);
        }
        else {
            shader->setUniform1i("u_mode", (int)addMode);
        }

    }

    mutable ofFbo maskFbo;
    ADD_MODE addMode;
    bool b_isActive;
    bool b_isEnabled;

    shared_ptr<AutoShader> shader;
};
class QuadMask : public Mask {
public:
    void draw(pingPongFbo & _fbo, glm::vec2 _position, float scale, bool equals = false) const {
        begin();
        ofClear(0);
        ofBackground(255);
        quadFbo.draw(   
            _position.x - 0.5 * scale * quadFbo.getWidth(),
            _position.y - 0.5 * scale * quadFbo.getHeight(),
            scale * quadFbo.getWidth(),
            scale * quadFbo.getHeight()
        ); 
        end();

        Mask::draw(_fbo, equals);
    }

    virtual void setupQuad(int width, int height) { 
        quadFbo.allocate(width, height, GL_RGBA); 
        quadFbo.begin();
        ofClear(0);
        ofBackground(255);
        quadFbo.end();
    }

    void beginQuad() const { quadFbo.begin(); }
    void endQuad()   const { quadFbo.end();   }

protected:

    mutable ofFbo quadFbo;

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
                if (_active) {
                    if (_mask == "Custom Mask") onCustomMaskActive(_active);
                    setEnabled(_mask, true);
                }
            }
            else {
                mask.second->setActive(false);
            }
        }
        redraw();
    }
    void setEnabled(const string & _mask, bool _active) {
        for (auto & mask : masks) {
            if (mask.first == _mask) {
                mask.second->setEnabled(_active);
                if (_mask == "Custom Mask") onCustomMaskActive(_active);
                redraw();
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
    
    bool isEnabled(const string & _mask) {
        auto mask = masks.find(_mask);
        if (mask != masks.end()) {
            return (mask->second->isEnabled());
        }
        return false;
    }

    Mask::ADD_MODE  getMaskMode(const string & _mask) { 
        auto mask = masks.find(_mask);
        if (mask != masks.end()) {
            return (mask->second->getAddMode());
        }
        return Mask::ADD_MODE::AND;
    }

    vector<string> getMasks() { 
        vector<string> maskKeys;
        maskKeys.reserve(masks.size());
        for (auto const& maskEl : masks) maskKeys.push_back(maskEl.first);
        return maskKeys;  
    }

protected:
    virtual void onSetup()       override;
    virtual void onDrawGui()     override;
    virtual void onDrawOverlay() override;
    virtual void onRender()      const override;
    virtual void onResize()      override;

    virtual void setupMaskComposeFbo();
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
    Mask        customMask;

private:
    ofxCv::ObjectFinder finder;

    mutable pingPongFbo maskComposeFbo;

    // Custom Draw
    float brushSize = 50;
    const float maxBrushSize = 200;
    const float minBrushSize = 10;
    glm::vec2 brushPosition;
};

