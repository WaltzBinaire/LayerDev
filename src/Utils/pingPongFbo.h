#pragma once

#include "ofMain.h"

class pingPongFbo {
public:
        
    void allocate(int width, int height, int internalformat = GL_RGBA, int numSamples = 0) {
        ofFboSettings settings;
        settings.numColorbuffers = 1;   // one buff per fbo
        settings.width              = width;
        settings.height             = height;
        settings.internalformat     = internalformat;
        settings.wrapModeHorizontal = true;
        settings.wrapModeVertical   = true;
        settings.numSamples         = numSamples;
        settings.useDepth           = false;
        settings.useStencil         = false;
        settings.textureTarget      = GL_TEXTURE_2D;
        allocate(settings);
    }
        
    bool isAllocated() const {
        return fbos[0].isAllocated() && fbos[1].isAllocated();
    }

    void allocate(ofFboSettings _settings) {
        fbos[0].allocate(_settings);
        fbos[1].allocate(_settings);
        flag = false;
    }
        
    void swap(){
        flag = !flag;
    }
        
    ofFbo& get() { return fbos[flag]; }
    ofFbo& getBack() { return fbos[!flag]; }
        
    ofTexture& getTexture()     const { return fbos[flag].getTexture(); }
    ofTexture& getBackTexture() const { return fbos[!flag].getTexture(); }

    void bind()   { fbos[flag].bind();   }
    void unbind() { fbos[flag].unbind(); }
    void begin()  { fbos[flag].begin();  }
    void end()    { fbos[flag].end();    }

    void clear()    { clear(get());         }
    void clearBack(){ clear(getBack());     }
    void clearAll() { clear(); clearBack(); }

    float getWidth() { return fbos[flag].getWidth(); }
    float getHeight() { return fbos[flag].getHeight(); }
        
    void draw(float x, float y, float w, float h) const     { fbos[flag].draw(x,y,w,h); }
    void drawBack(float x, float y, float w, float h) const { fbos[!flag].draw(x,y,w,h); }
    void draw(float x, float y)                             { fbos[flag].draw(x,y); }
    void drawBack(float x, float y)                         { fbos[!flag].draw(x,y); }

private:
    mutable ofFbo fbos [2];
    bool flag;

    void clear(ofFbo & _fbo) {
        _fbo.begin();
        ofClear(0);
        _fbo.end();
    }
        
};