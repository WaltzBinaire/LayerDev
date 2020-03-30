#include "LayerVisualiser.h"
#include "LayerUtils.h"

using namespace LayerUtils;

LayerVisualiser::LayerVisualiser()
{
    setupFbo();
    shader         = Shader_lib::get_passthrough_shader();

    l_onWindowResized = ofEvents().windowResized.newListener([=](ofResizeEventArgs & _args) {this->setupFbo(); });
}

LayerVisualiser::~LayerVisualiser()
{
}

void LayerVisualiser::draw(const deque<shared_ptr<Layer_base>> & layers) const
{
    const float maxXOffset = 1000;
    const float maxYOffset = 1000;
    const float maxZOffset = -100;



    fbo.begin();
    ofBackground(ofColor::black);

    float mouseX = (float)ofGetMouseX() / ofGetWidth();
    float mouseY = (float)ofGetMouseY() / ofGetHeight();

    int  numLayers = layers.size();
    auto quad = UVQuad::getInstance();

    shader->begin();
    cam.begin();
    cam.setVFlip(true);
    cam.setPosition(glm::vec3(0, 0, 1000));
    for (int i = 0; i < layers.size(); i++) {
        float f = (float)i/ layers.size();
        float xOffset = i * ofLerp( -maxXOffset, maxXOffset, mouseX );
        float yOffset = i * ofLerp( -maxYOffset, maxYOffset, mouseY );
        float zOffset = ofMap(i, 0, layers.size() - 1, 0, maxZOffset);

        const Layer_base & layer = *(layers[i]);

        if (layer.isEnabled()) {
            shader->setUniformTexture("u_imageTex", layer.getContentTexture(), 0);

            glm::vec2 size( layer.getWidth(), layer.getHeight());
            size *= min(fbo.getWidth() / layer.getWidth(), fbo.getHeight() / layer.getHeight());

            glm::vec2 pos = -0.5 * size;

            ofPushMatrix();
            ofTranslate(xOffset, yOffset, zOffset);

            quad.draw(pos, size);

            ofPopMatrix();

        }
    }
    cam.end();
    shader->end();
    fbo.end();

    fbo.draw(0, 0);
}

void LayerVisualiser::setupFbo()
{
        fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
}
