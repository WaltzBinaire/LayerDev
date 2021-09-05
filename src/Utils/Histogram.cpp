#include "Histogram.h"

Histogram::Histogram()
{
    plane.set(FBO_RESOLUTION_X, FBO_RESOLUTION_Y);   
    plane.setResolution(PLANE_RESOLUTION, PLANE_RESOLUTION);
    plane.mapTexCoords(0, 0, 1, 1);
    plane.rotate(65, glm::vec3(1, 0, 0));


    ofFboSettings settings;
    settings.numColorbuffers    = 1;   // one buff per fbo
    settings.width              = FBO_RESOLUTION_X;
    settings.height             = FBO_RESOLUTION_Y;
    settings.internalformat     = GL_RGBA;
    settings.wrapModeHorizontal = true;
    settings.wrapModeVertical   = true;
    settings.numSamples         = 0;
    settings.useDepth           = false;
    settings.useStencil         = false;
    settings.textureTarget      = GL_TEXTURE_2D;
    fbo.allocate(settings);

    cam.setupPerspective(true, 30, 0.0, 1000);

    shader = Shader_lib::get_histogram_shader();

    l_onUpdate = ofEvents().update.newListener([this](ofEventArgs &) { this->render(); });
}


Histogram::~Histogram()
{
}

void Histogram::render()
{
    if (!texture.isAllocated()) return;

    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    fbo.begin();
    ofClear(0.0, 0.0);

    cam.begin();
    cam.setPosition(0, 0, FBO_RESOLUTION_X * 1.5);

    drawChannel(CHANNEL::RED);
    drawChannel(CHANNEL::GREEN);
    drawChannel(CHANNEL::BLUE);

    cam.end();
    fbo.end();

    ofPopStyle();
}

void Histogram::draw() const
{
    fbo.draw(0, 0);
}

void Histogram::setTextureSource(ofTexture & _tex)
{
    texture = _tex;
}

void Histogram::drawChannel(CHANNEL ch) const
{
    shader->begin();
    shader->setUniformTexture("u_imageTex", texture, 0);
    shader->setUniform1i("u_channel", (int)ch);
    shader->setUniform1f("u_scale" , FBO_RESOLUTION_Y * 0.33  );
    plane.draw();

    shader->end();
}
