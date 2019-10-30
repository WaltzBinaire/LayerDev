#include "Histogram.h"

Histogram::Histogram()
{
    plane.set(FBO_RESOLUTION_X, FBO_RESOLUTION_Y);   
    plane.setResolution(PLANE_RESOLUTION, PLANE_RESOLUTION);
    plane.mapTexCoords(0, 0, 1, 1);
    plane.rotate(65, glm::vec3(1, 0, 0));


    fbo.allocate(FBO_RESOLUTION_X, FBO_RESOLUTION_Y, GL_RGBA);

    cam.setupPerspective(true, 30, 0.0, 1000);

    shader = Shader_lib::get_histogram_shader();
}


Histogram::~Histogram()
{
}

void Histogram::render()
{
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

void Histogram::setTexture(ofTexture & _tex)
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
