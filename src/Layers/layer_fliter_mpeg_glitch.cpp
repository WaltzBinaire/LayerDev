#include "Layers\layer_fliter_mpeg_glitch.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_filter_mpeg_glitch, MPEG Glitch)

void Layer_filter_mpeg_glitch::onSetup()
{
    Layer_filter_shader::onSetup();

    createNoiseTexture();
}

void Layer_filter_mpeg_glitch::createNoiseTexture()
{
    ofPixels noisePixels;
    noiseTex.allocate(noiseTexSize, noiseTexSize, GL_RGB);
    noiseTex.setTextureWrap(GL_REPEAT, GL_REPEAT);
    noisePixels.allocate(noiseTexSize, noiseTexSize, OF_PIXELS_RGB);

    for (int x = 0; x < noiseTexSize; x++) {
        for (int y = 0; y < noiseTexSize; y++) {
            noisePixels.setColor(x, y, ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255)));
        }
    }
    noiseTex.loadData(noisePixels);
}

void Layer_filter_mpeg_glitch::onUpdate()
{
    if (!p_pause) {
        redraw();
    }
}

void Layer_filter_mpeg_glitch::onDrawGui()
{
    SingleLayerGui::specialisedDrawGui<Layer_filter_mpeg_glitch>(this); 
}

void Layer_filter_mpeg_glitch::onSetupParams()
{
    p_sizeOfKernel.set  ("Size of Kernel", 1.0, 0.0, 1.0);
    p_stopAmount.set    ("Stop Amount"   , 0.5, 0.0, 1.0);
    p_globalStrength.set("Strength"      , 0.5, 0.0, 1.0);
    p_greyScale.set     ("Greyscale"     , false);

    params.add(
        p_sizeOfKernel,
        p_stopAmount,
        p_globalStrength,
        p_greyScale
    );

}

void Layer_filter_mpeg_glitch::setupShader()
{
    shader = Shader_lib::get_mpeg_glitch_shader();
    //shader = Shader_lib::get_passthrough_shader();
}

void Layer_filter_mpeg_glitch::setUniforms(const ofTexture & _baseTex) const
{
    shader->setUniformTexture("u_imageTex" , _baseTex, 0);
    shader->setUniformTexture("u_noiseTex" , noiseTex, 1);
    shader->setUniform2f("u_resolution"    , size.x, size.y);
    shader->setUniform1f("u_sizeOfKernel" , p_sizeOfKernel         );
    shader->setUniform1f("u_stopAmount"    , p_stopAmount           );
    shader->setUniform1f("u_globalStrength", p_globalStrength       );
    shader->setUniform1i("u_greyScale"     , p_greyScale            );
    shader->setUniform1f("u_time"          , time                   );
}
