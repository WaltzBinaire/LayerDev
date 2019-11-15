#include "Canvas.h"


const ofColor Canvas::defaultBackground(0, 0, 0, 255);

Canvas::Canvas() :
    backgroundColor(defaultBackground),
    position(0, 0),
    scale(1)
{
    size = getCanavasSizePresets()[0].second;
}


Canvas::~Canvas()
{
}

void Canvas::setup()
{
    setupParams();
    setupFbo();
}

void Canvas::resize(float width, float height)
{
    if (width <= 0 || height <= 0) return;

    glm::vec2 _size = glm::vec2(width, height);

    if (size != _size) {
        size = _size;
        setupFbo();
        canvasResized.notify(size);
    }
}

ofMouseEventArgs Canvas::transformMouseEventArgs(const ofMouseEventArgs & _args) {
    ofMouseEventArgs args(_args);
    args.x = (args.x - position.x) / scale ;
    args.y = (args.y - position.y) / scale ;
    return args;
}

float * Canvas::getBackgroundColorArray() const {
    return new float[4]{
        (float)(backgroundColor.r / 255.0),
        (float)(backgroundColor.g / 255.0),
        (float)(backgroundColor.b / 255.0),
        (float)(backgroundColor.a / 255.0)
    };
}

ofColor Canvas::getBackgroundColor() const
{
    return backgroundColor;
}

ofPixels Canvas::getPixels() const
{
    ofPixels pixels;
    fbo.getTexture().readToPixels(pixels);
    return pixels;
}


void Canvas::draw(bool drawOverlay) const
{
    fitToScreen();

    fbo.draw(
        position.x, 
        position.y,
        size.x * scale,
        size.y * scale
    );

    ofPushStyle();
    ofNoFill();  

    if (drawOverlay) {
        overlay.draw(
            position.x, 
            position.y,
            size.x * scale,
            size.y * scale
        );
    }


    if(backgroundColor.getBrightness() > 0.5) ofSetColor(ofColor::black); 
    else ofSetColor(ofColor::white); 

    ofDrawRectangle(
        position.x, 
        position.y,
        size.x * scale,
        size.y * scale);

    ofPopStyle();
}

void Canvas::setBackgroundColor(const ofColor _backgroundColor)
{
    backgroundColor = _backgroundColor;
    static bool changed = true;
    backgroundChanged.notify(changed);
}

void Canvas::setupFbo()
{
    fbo.allocate(size.x, size.y);
    overlay.allocate(size.x, size.y);
    clear();    
}

void Canvas::setupParams()
{
    p_autoResize.addListener(this, &Canvas::onAutoResizeChanged);
    p_autoResize.set("Auto Resize", false);
}

void Canvas::fitToScreen() const
{

    float sw = ofGetWidth()  / getWidth();
    float sh = ofGetHeight() / getHeight();

    scale = min( 1.0f, min(sw, sh)) * 0.9;

    position = glm::vec2(
        (ofGetWidth()  * 0.5) - (0.5  * scale * getWidth()  ),
        (ofGetHeight() * 0.5) - (0.5  * scale * getHeight())
    );

}

void Canvas::clear()
{
    clearContent();
    clearOverlay();
}

void Canvas::clearOverlay()
{

    overlay.begin();
    ofClear(0.0);
    overlay.end();
}

void Canvas::clearContent()
{
    fbo.clearAll();
    fbo.begin();
    ofBackground(backgroundColor);
    fbo.end();
}

void Canvas::onAutoResizeChanged(bool & _val)
{
    if (_val) {
        ofAddListener(ofEvents().windowResized, this, &Canvas::onWindowResized);
    }
    else {
        ofRemoveListener(ofEvents().windowResized, this, &Canvas::onWindowResized);
    }
}

void Canvas::onWindowResized(ofResizeEventArgs & _args)
{
    resize(_args.width, _args.height);
}
