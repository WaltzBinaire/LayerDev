#include "Layers\layer_filter_shader.h"


void Layer_filter_shader::onDraw(const ofTexture & _baseTex, bool _forced) const
{
    
    if (!shader->isLoaded()) {
        ofLogWarning(name) << "Shader not loaded";
        ofSetColor(ofColor::cyan);
        ofDrawRectangle(0, 0, _baseTex.getWidth(), _baseTex.getHeight());
    }

    shader->begin();

    setUniforms(_baseTex);
    setMesh( _baseTex);
    mesh.draw();

    shader->end();
}

void Layer_filter_shader::onSetup()
{
    setupMesh();
    setupShader();
    l_onShaderLoad = shader->onLoad.newListener( [&](bool &) {return this->redraw(); });
}

void Layer_filter_shader::setupMesh()
{
	mesh.addVertex(glm::vec3(0));
	mesh.addVertex(glm::vec3(0));
	mesh.addVertex(glm::vec3(0));
	mesh.addVertex(glm::vec3(0));
			
	mesh.addTexCoord(glm::vec2(0.0, 0.0));
	mesh.addTexCoord(glm::vec2(1.0, 0.0));
	mesh.addTexCoord(glm::vec2(1.0, 1.0));
	mesh.addTexCoord(glm::vec2(0.0, 1.0));

    mesh.addIndex(0);
    mesh.addIndex(1);
    mesh.addIndex(2);

    mesh.addIndex(2);
    mesh.addIndex(3);
    mesh.addIndex(0);
}

void Layer_filter_shader::setUniforms(const ofTexture & _baseTex) const
{
    _baseTex.bind();
    shader->setUniform2f("u_resolution", _baseTex.getWidth(), _baseTex.getHeight());
}

void Layer_filter_shader::setMesh(const ofTexture & _baseTex) const
{
	mesh.setVertex(0, glm::vec3( 0                  , 0                   , 0 ));
	mesh.setVertex(1, glm::vec3( _baseTex.getWidth(), 0                   , 0 ));
	mesh.setVertex(2, glm::vec3( _baseTex.getWidth(), _baseTex.getHeight(), 0 ));
	mesh.setVertex(3, glm::vec3( 0                  , _baseTex.getHeight(), 0 ));
}
