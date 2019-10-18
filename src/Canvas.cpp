#include "Canvas.h"


const ofColor Canvas::defaultBackground(0, 0, 0, 255);

Canvas::Canvas() :
    backgroundColor(defaultBackground),
    size(1024, 780),
    position(0, 0),
    scale(1)
{
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
    size = glm::vec2(width, height);
    setupFbo();
    canvasResized.notify(size);
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


void Canvas::draw() const
{
    fbo.draw(
        position.x, 
        position.y,
        size.x * scale,
        size.y * scale
    );
}

void Canvas::setBackgroundColor(float _backgroundColor[4]) {
    backgroundColor = ofColor(
        _backgroundColor[0] * 255.0,
        _backgroundColor[1] * 255.0,
        _backgroundColor[2] * 255.0,
        _backgroundColor[3] * 255.0
    );
}

void Canvas::setBackgroundColor(const ofColor _backgroundColor)
{
    backgroundColor = ofColor(_backgroundColor);
}

void Canvas::setupFbo()
{
    fbo.allocate(size.x, size.y);
    clear();    
}

void Canvas::setupParams()
{
    p_autoResize.addListener(this, &Canvas::onAutoResizeChanged);
    p_autoResize.set("Auto Resize", true);
}

void Canvas::clear() 
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
