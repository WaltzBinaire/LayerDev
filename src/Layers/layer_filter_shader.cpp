#include "Layers\layer_filter_shader.h"
#include "Utils/LayerUtils.h"

void Layer_filter_shader::onDraw(const ofTexture & _baseTex, bool _forced) const
{
    
    if (!shader->isLoaded()) {
        ofLogWarning(name) << "Shader not loaded";
        ofSetColor(ofColor::cyan);
        ofDrawRectangle(0, 0, _baseTex.getWidth(), _baseTex.getHeight());
    }

    shader->begin();

    setUniforms(_baseTex);
    LayerUtils::UVQuad::getInstance().draw(0, 0, size.x, size.y);

    shader->end();
}

void Layer_filter_shader::onSetup()
{
    setupShader();
    l_onShaderLoad = shader->onLoad.newListener( [&](bool &) {return this->redraw(); });
}


void Layer_filter_shader::setUniforms(const ofTexture & _baseTex) const
{
    _baseTex.bind();
    shader->setUniform2f("u_resolution", _baseTex.getWidth(), _baseTex.getHeight());
}
