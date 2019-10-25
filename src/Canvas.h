#pragma once
#include "ofMain.h"
#include "Utils/pingPongFbo.h"

class Canvas
{
public:
    Canvas();
    ~Canvas();

    void setup();
    void resize(float width, float height);
    void resize(glm::vec2 size) { resize(size.x, size.y); };

    void fitToScreen() const;

    void clear();
        
    void draw(bool drawOverlay = true) const;

    void setBackgroundColor(const ofColor _backgroundColor);

    float*  getBackgroundColorArray()  const;
    ofColor getBackgroundColor()       const;

    ofPixels getPixels()   const;
    pingPongFbo & getFbo() const {
        return fbo;
    };
    ofFbo & getOverlayFbo() const {
        return overlay;
    };

    float getWidth()   const { return size.x;} ;
    float getHeight()  const { return size.y;} ;

    ofEvent<glm::vec2> canvasResized;
    ofEvent<bool> backgroundChanged;

    static const ofColor defaultBackground;
    ofParameter<bool> p_autoResize;
private:

    Canvas(Canvas const&)  = delete;
    void operator=(Canvas const&)  = delete;

    void setupFbo();
    void setupParams();

    void onAutoResizeChanged(bool & _val);
    void onWindowResized(ofResizeEventArgs & _args);

    glm::vec2 size;
    ofColor backgroundColor;

    // Params


    // Drawing
    mutable pingPongFbo fbo;
    mutable ofFbo       overlay;
    mutable glm::vec2 position;
    mutable float scale;


};

