#include "Layers\layer_filter_chromatic_aberation.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_filter_chromatic_aberation)

void Layer_filter_chromatic_aberation::onDrawGui(){
    SingleLayerGui::specialisedDrawGui<Layer_filter_chromatic_aberation>(this); 
}

void Layer_filter_chromatic_aberation::onSetupParams()
{
    p_amount.set("Amount", 0.1, 0.0, 1.0);
    p_angle.set("Angle"  , 0.0, 0.0, 360.0);

    params.add(
        p_amount,
        p_angle
    );

}

void Layer_filter_chromatic_aberation::setupShader()
{
    shader = Shader_lib::get_chromatic_shader();
}

void Layer_filter_chromatic_aberation::setUniforms(const ofTexture & _baseTex) const
{
    shader->setUniformTexture("u_imageTex", _baseTex, 0);
    shader->setUniform2f("u_resolution", _baseTex.getWidth(), _baseTex.getHeight());
    shader->setUniform1f("u_amount", p_amount);
    shader->setUniform1f("u_angle", p_angle);
}
