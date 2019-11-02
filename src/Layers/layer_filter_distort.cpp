#include "Layers\layer_filter_distort.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_filter_distort, Glitch Distort)

void Layer_filter_distort::onRender(const ofTexture & _baseTex) const
{
    drawBrush(_baseTex);
}

void Layer_filter_distort::onDrawGui()
{
    SingleLayerGui::specialisedDrawGui<Layer_filter_distort>(this); 
}

void Layer_filter_distort::onSetup()
{
    Layer_filter_shader::onSetup();
    setupBrushFbo();
}

void Layer_filter_distort::onReset()
{
    brushFbo.clearAll();
    b_brushFboInitialised = false;
}

void Layer_filter_distort::onUpdate()
{
    if (b_drawing) {
        redraw();
    }
}


void Layer_filter_distort::onSetupParams()
{
    p_blur.set("Blur", 0.4, 0.0, 1.0);
    p_size.set("Size", 0.5, 0.0, 1.0);
    p_shape.set("Shape"  , (int)SHAPE::CIRCLE, (int)SHAPE::CIRCLE, (int)SHAPE::RECTANGLE);

    params.add(
        p_size,
        p_blur,
        p_shape
    );
}

void Layer_filter_distort::onResize()
{
    setupBrushFbo();
}

void Layer_filter_distort::onSetupListeners()
{
    l_onMousePressed  = layer_manager->canvasMousePressed .newListener( this, &Layer_filter_distort::onMousePressed );
    l_onMouseDragged  = layer_manager->canvasMouseDragged .newListener( this, &Layer_filter_distort::onMouseDragged );
    l_onMouseReleased = layer_manager->canvasMouseReleased.newListener( this, &Layer_filter_distort::onMouseReleased);
    l_onMouseScrolled = layer_manager->canvasMouseScrolled.newListener( this, &Layer_filter_distort::onMouseScrolled);
}


void Layer_filter_distort::onMousePressed(ofMouseEventArgs & _args)
{
    if (!b_drawing) {
        mousePressedPosition = mousePosition = glm::vec2(_args.x, _args.y);
    }
    else {
        glm::vec2 movement = glm::vec2(_args.x, _args.y) - mousePosition;
        mousePosition += movement * 0.8;
    }
    b_drawing = true;
}

void Layer_filter_distort::onMouseDragged(ofMouseEventArgs & _args)
{
    glm::vec2 movement = glm::vec2(_args.x, _args.y) - mousePosition;
    mousePosition += movement * 0.4;
}

void Layer_filter_distort::onMouseReleased(ofMouseEventArgs & _args)
{
    b_drawing = false;
}

void Layer_filter_distort::onMouseScrolled(ofMouseEventArgs & _args)
{
    p_size += 0.05 * _args.scrollY;
    p_size = ofClamp(p_size, p_size.getMin(), p_size.getMax());
}

void Layer_filter_distort::setupShader()
{
    shader         = Shader_lib::get_uv_distort_shader();
    uv_draw_shader = Shader_lib::get_uv_draw_shader();
    l_onUvShaderLoad = uv_draw_shader->onLoad.newListener([&](bool &) {return  b_brushFboInitialised = false;  this->redraw(); });
}

void Layer_filter_distort::setUniforms(const ofTexture & _baseTex) const
{
    shader->setUniformTexture("u_imageTex", _baseTex , 0);
    shader->setUniformTexture("u_uvMap"   , brushFbo.getTexture() , 1);
}

void Layer_filter_distort::setupBrushFbo()
{
    ofFboSettings settings;
    settings.numColorbuffers = 1;   // one buff per fbo
    settings.width              = fbo.getWidth();
    settings.height             = fbo.getHeight();
    settings.internalformat     = GL_RGBA16F;
    settings.wrapModeHorizontal = true;
    settings.wrapModeVertical   = true;
    settings.numSamples         = 0;
    settings.useDepth           = false;
    settings.useStencil         = false;
    settings.textureTarget      = GL_TEXTURE_2D;
    brushFbo.allocate(settings);
}

void Layer_filter_distort::drawBrush(const ofTexture & _baseTex) const
{
    brushFbo.swap();
    brushFbo.begin();
    uv_draw_shader->begin();

    glm::vec2 pos    = mousePosition        / glm::vec2(_baseTex.getWidth(), _baseTex.getHeight());
    glm::vec2 srtPos = mousePressedPosition / glm::vec2(_baseTex.getWidth(), _baseTex.getHeight());

    uv_draw_shader->setUniformTexture("u_imageTex", brushFbo.getBackTexture(), 0);
    uv_draw_shader->setUniform2f("u_resolution", _baseTex.getWidth(), _baseTex.getHeight());
    uv_draw_shader->setUniform2f("u_srtPos" , srtPos);
    uv_draw_shader->setUniform2f("u_pos"    , pos);
    uv_draw_shader->setUniform1f("u_size"   , p_size);
    uv_draw_shader->setUniform1f("u_blur"   , p_size * p_blur);
    uv_draw_shader->setUniform1i("u_shape"  , p_shape);
    uv_draw_shader->setUniform1i("u_drawing", (int)b_drawing);
    uv_draw_shader->setUniform1i("u_initialized", (int)b_brushFboInitialised);

    setMesh( _baseTex);
    mesh.draw();

    uv_draw_shader->end();
    brushFbo.end();
    b_brushFboInitialised = true;
}
