#pragma once
#include "Layers/layer_base.h"
#include "Utils/shader_base.h"

class Layer_filter_shader : public Filter_base
{
public:
    Layer_filter_shader(string name, int instance, Layer_Manager * _layer_manager) : Filter_base(name, instance, _layer_manager) {};

protected:
    virtual void onDraw(const ofTexture & _baseTex, bool _forced = false) const override;
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

