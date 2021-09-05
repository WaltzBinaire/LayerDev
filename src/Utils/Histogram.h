#pragma once
#include "ofMain.h"
#include "Utils/shader_base.h"

#define PLANE_RESOLUTION 100
#define FBO_RESOLUTION_X 300
#define FBO_RESOLUTION_Y 200

class Histogram
{
public:

    enum class CHANNEL {RED = 0, GREEN = 1, BLUE = 2};

    Histogram();
    ~Histogram();

    void render();
    void draw() const;

    template<class T>
    void setFboSource(T fbo) {
        texture = fbo.getTexture();
    };

    void setTextureSource(ofTexture & _tex);

    ofTexture & getTexture() {
        return fbo.getTexture();
    }

private:

    ofEventListener l_onUpdate;

    void drawChannel(CHANNEL ch) const;

    ofPlanePrimitive plane;
    ofCamera cam;
    ofFbo fbo;
    mutable shared_ptr<AutoShader> shader;

    ofTexture texture;
};

