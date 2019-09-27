#pragma once
#include "layer_base.h"
#include "shader_base.h"

class Layer_filter_shader : public Filter_base
{
public:
    Layer_filter_shader(string name, Layer_Manager * _layer_manager) : Filter_base(name, _layer_manager) {};

protected:
    virtual void onDraw(const ofTexture & _baseTex) const override;
    virtual void onSetup()  override;

    virtual void setupMesh();
    virtual void setupShader() = 0;
    virtual void setUniforms(const ofTexture & _baseTex) const;
    virtual void setMesh(const ofTexture & _baseTex) const;

    shared_ptr<AutoShader> shader;
    mutable ofMesh mesh;

private:
    ofEventListener l_onShaderLoad;
};

