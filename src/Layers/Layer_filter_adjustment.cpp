#include "Layers/Layer_filter_adjustment.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_filter_adjustment, Adjustments)

void Layer_filter_adjustment::onDrawGui(){
    SingleLayerGui::specialisedDrawGui<Layer_filter_adjustment>(this); 
}

void Layer_filter_adjustment::onSetupParams()
{
    p_resetAdjustments.addListener(this, &Layer_filter_adjustment::onResetAdj);

    p_brightness.set("Brightness", 0.0, -1.0, 1.0);
    p_contrast.set  ("Contrast"  , 1.0, 0.5, 2.0);
    p_saturation.set("Saturation", 1.0, 0.0, 2.0);
    p_vibrance.set  ("Vibrance"  , 1.0, 0.0, 2.0);
    p_hue.set       ("Hue"       , 0.0, 0.0, 1.0);
    p_resetAdjustments.set("Reset All", false);

    params.add(
        p_brightness,
        p_contrast,
        p_saturation,
        p_vibrance,
        p_hue,
        p_resetAdjustments
    );

}

void Layer_filter_adjustment::setupShader()
{
    shader = Shader_lib::get_postprocessing_shader();
}

void Layer_filter_adjustment::setUniforms(const ofTexture & _baseTex) const
{
    shader->setUniformTexture("u_imageTex", _baseTex, 0);
    shader->setUniform1f("u_brightness", p_brightness);
    shader->setUniform1f("u_contrast"  , p_contrast);
    shader->setUniform1f("u_saturation", p_saturation);
    shader->setUniform1f("u_vibrance"  , p_vibrance);
    shader->setUniform1f("u_hue"       , p_hue);
}

void Layer_filter_adjustment::onResetAdj(bool & _val)
{
    if (!_val) return;
    _val = false;

    p_brightness.set(0.0);
    p_contrast.set  (1.0);
    p_saturation.set(1.0);
    p_vibrance.set  (1.0);
    p_hue.set       (0.0);
}
