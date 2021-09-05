#include "Layers\layer_alpha_replace_chanel.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_alpha_replace_channel, Channel Apha)

void Layer_alpha_replace_channel::onRender(const ofTexture & _baseTex, bool _forced) const
{
    renderReplacmentFbo();
    update_alpha_texture(_baseTex);
}

void Layer_alpha_replace_channel::onDrawGui()
{
    Layer_filter_alpha_replace::onDrawGui();
    SingleLayerGui::specialisedDrawGui<Layer_alpha_replace_channel>(this); 
}

void Layer_alpha_replace_channel::onSetupParams()
{
    Layer_filter_alpha_replace::onSetupParams();

    p_invert.set    ("Invert"     , false);
    p_threshold.set ("Threshold"  , glm::vec2(0.5),  glm::vec2(0.0),  glm::vec2(1.0));
    p_blurPasses.set("Blur Passes", 4, 1, 10);
    p_color.set     ("Channel Mix", glm::vec4(0.5), glm::vec4(0), glm::vec4(1));

    params.add(
        p_invert,
        p_color,
        p_threshold,
        p_blurPasses
    );
}

void Layer_alpha_replace_channel::setupShader()
{
    Layer_filter_alpha_replace::setupShader();

    blur_shader  = Shader_lib::get_blur_shader();
    l_onBlurShaderLoad = blur_shader->onLoad.newListener([&](bool &) {return  redraw(); });

    threshold_shader  = Shader_lib::get_thresh_shader();
    l_onTheshShaderLoad = blur_shader->onLoad.newListener([&](bool &) {return  redraw(); });

}

void Layer_alpha_replace_channel::update_alpha_texture(const ofTexture & _baseTex) const
{
    if (!alphaBlur.isAllocated()) alphaBlur.allocate((int)_baseTex.getWidth(), (int)_baseTex.getHeight(), GL_RGBA);
    
    if (alphaBlur.getWidth() != _baseTex.getWidth() || alphaBlur.getHeight() != _baseTex.getHeight()) 
        alphaBlur.allocate((int)_baseTex.getWidth(), (int)_baseTex.getHeight(), GL_RGBA);


    blur_shader->begin();

    const ofTexture * tex = &_baseTex;

    for (int i = 0; i < p_blurPasses; i++) {

        alphaBlur.begin();

        blur_shader->setUniformTexture("u_imageTex", *tex , 0);
        blur_shader->setUniform2f("u_resolution", _baseTex.getWidth(), _baseTex.getHeight());
        blur_shader->setUniform2f("u_direction", glm::vec2(1.0, 0.0));
        LayerUtils::UVQuad::getInstance().draw(0, 0, size.x, size.y);

        alphaBlur.end();

        alphaBlur.swap();
        tex =  &alphaBlur.getBackTexture();

        alphaBlur.begin();
        
        blur_shader->setUniformTexture("u_imageTex", *tex , 0);
        blur_shader->setUniform2f("u_resolution", _baseTex.getWidth(), _baseTex.getHeight());
        blur_shader->setUniform2f("u_direction", glm::vec2(0.0, 1.0));
        LayerUtils::UVQuad::getInstance().draw(0, 0, size.x, size.y);

        alphaBlur.end();
        alphaBlur.swap();

        tex = &alphaBlur.getBackTexture();
    }

    blur_shader->end();


    alphaBlur.begin();
    threshold_shader->begin();
    
    blur_shader->setUniformTexture("u_imageTex", *tex , 0);
    threshold_shader->setUniform2f("u_thresh", p_threshold);
    threshold_shader->setUniform1i("u_invert", p_invert);
    threshold_shader->setUniform4f("u_color",  p_color);

    LayerUtils::UVQuad::getInstance().draw(0, 0, size.x, size.y);

    threshold_shader->end();
    alphaBlur.end();
    alphaBlur.swap();

}
