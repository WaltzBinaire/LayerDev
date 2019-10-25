#include "Layers\layer_base.h"
#include "GUI\SingleLayerGui.h"
#include "Utils\LayerUtils.h"

Layer_base::Layer_base(string _name, int _instance, Layer_Manager * layer_manager) :
    name(_name),
    instance(_instance),
    b_active(false),
    layer_manager(layer_manager)
{
    quadSetup();
}

Layer_base::~Layer_base()
{
    onDestroy();
}

void Layer_base::setup(int  _width, int _height) {
    size = glm::vec2(_width, _height);
    setupFbo(size.x, size.y);

    l_onShaderLoad = mask_shader->onLoad.newListener( [&](bool &) {return this->redraw(); });

    onSetup();

    p_reset.addListener(this, &Layer_base::onResetInternal);
    
    p_reset.set("Reset", false);
    p_disable.set("Disable", false);
    p_debugRedraw.set("Redraw", false);   

    p_loadMask.addListener(this, &Layer_base::onLoadMask);
    p_clearMask.addListener(this, &Layer_base::onClearMask);
    p_loadMask.set(  "Load Mask"  , false);
    p_clearMask.set( "Clear Mask" , false);
    p_showMask.set(  "Show Mask"  , false);
    p_mask.set(      "Mask"       , false);
    p_invertMask.set("Invert Mask", false);
    
    p_pause.set("Pause"  , false);
    
    params.add(
        p_reset,
        p_disable,
        p_debugRedraw,
        p_loadMask,
        p_clearMask,
        p_showMask,
        p_mask,
        p_invertMask,
        p_pause
    );

    l_paramsChanged = params.parameterChangedE().newListener([&](ofAbstractParameter &) {return this->redraw(); }, OF_EVENT_ORDER_BEFORE_APP);

    onSetupParams();
}

void Layer_base::drawGui() 
{
    SingleLayerGui::drawGui(this); 
    onDrawGui();
}

void Layer_base::drawOverlay(ofFbo & overlayFbo)
{
    if (p_disable) return;
    overlayFbo.begin();
    if(p_showMask) maskFbo.draw(0, 0);
    onDrawOverlay();
    overlayFbo.end();
}

void Layer_base::saveLayer()
{
    string newPath;
    if (LayerUtils::saveImageDialogue(newPath)) {
        LayerUtils::saveImage(newPath, fbo);
    }
}

void Layer_base::saveMask()
{
    string newPath;
    if (LayerUtils::saveImageDialogue(newPath)) {
        LayerUtils::saveImage(newPath, maskFbo);
    }
}

void Layer_base::registerType(const string & name, Layer_factory * factory)
{
    (Layer_base::GetFactoryDirectory())->insert_or_assign(name, factory);
}

Layer_base * Layer_base::create(const string & name, Layer_Manager * _layer_manager )
{
    return (Layer_base::GetFactoryDirectory())->at(name)->create(_layer_manager);
}

bool Layer_base::isFilter(const string & name)
{
    Layer_factory * factory = (Layer_base::GetFactoryDirectory())->at(name);

    return factory->isFilter();

}

vector<string> Layer_base::get_layer_names()
{
    vector<string> names;
    names.reserve((Layer_base::GetFactoryDirectory())->size());

    for (auto factory : *(Layer_base::GetFactoryDirectory())) {
        names.push_back(factory.first);
    }

    return names;
}

void Layer_base::setupFbo(int w, int h)
{  
    // Art fbo
    ofFboSettings settings;
    settings.numColorbuffers    = 1;   // one buff per fbo
    settings.width              = w;
    settings.height             = h;
    settings.internalformat     = GL_RGBA;
    settings.wrapModeHorizontal = true;
    settings.wrapModeVertical   = true;
    settings.numSamples         = 0;
    settings.useDepth           = false;
    settings.useStencil         = false;
    settings.textureTarget      = GL_TEXTURE_2D;
    fbo.allocate(settings);
    clearFbo();

    // Mask fbo
    ofFboSettings maskSettings;
    settings.numColorbuffers    = 1;   // one buff per fbo
    settings.width              = w;
    settings.height             = h;
    settings.internalformat     = GL_RGBA;
    settings.wrapModeHorizontal = true;
    settings.wrapModeVertical   = true;
    settings.numSamples         = 0;
    settings.useDepth           = false;
    settings.useStencil         = false;
    settings.textureTarget      = GL_TEXTURE_2D;
    maskFbo.allocate(settings);

}

void Layer_base::resize(int _width, int _height )
{
    size = glm::vec2(_width, _height);
    setupFbo(size.x, size.y);
    onResize();
    redraw();
}

void Layer_base::clearFbo() const
{
    fbo.begin();
    ofClear(0);
    fbo.end();
}

void Layer_base::clearMaskFbo() const
{
    maskFbo.begin();
    ofClear(0);
    ofBackground(255);
    maskFbo.end();
}

void Layer_base::onLoadMask(bool & _val)
{
    if (_val) {
        LayerUtils::loadFileDialogue(LayerUtils::img_exts, this, &Layer_base::handle_mask);
    }
    _val = false;
}

void Layer_base::onClearMask(bool & _val)
{
    if (_val) {
        clearMaskFbo();
    }
    _val = false;
}

void Layer_base::handle_mask(const string & _path)
{
    ofImage img;
    if (!img.load(_path)) {
        ofLogWarning(name) << "Could not open mask.";
    }
    else {
        maskFbo.begin();
        img.draw(0, 0, maskFbo.getWidth(), maskFbo.getHeight());
        maskFbo.end();
        ofLogVerbose(name) << "Mask loaded.";
    };
}

void Layer_base::onResetInternal(bool & b_reset) {
    if (b_reset) {
        reset();
        b_reset = false;
    }
}

bool Static_base::draw(pingPongFbo & mainFbo, bool _forceRedraw) const
{
    if (p_disable) {
        return needsRedraw();
    }
    else if (needsRedraw() || _forceRedraw) {

        onRender();

        fbo.begin();
        ofClear(0);
        onDraw();
        fbo.end();

        onMask();

        mainFbo.begin();
        fbo.draw(0, 0);
        mainFbo.end();
        setRedraw(false);
        return REDRAW;
    } else {            
        mainFbo.begin();
        fbo.draw(0, 0);
        mainFbo.end();
        return NO_REDRAW;
    }
}

bool Filter_base::draw(pingPongFbo & mainFbo, bool _forceRedraw) const
{
    if (p_disable ) {
        return needsRedraw();
    } else if (needsRedraw() || _forceRedraw) {
        mainFbo.swap();

        onRender(mainFbo.getBackTexture());

        fbo.begin();
        ofClear(0);
        onDraw(mainFbo.getBackTexture());
        fbo.end();

        setRedraw(false);

        onMask();
        
        mainFbo.begin();
        fbo.draw(0, 0);
        mainFbo.end();

        return REDRAW;
    }
    else {
        mainFbo.begin();
        fbo.draw(0, 0);
        mainFbo.end();
        return NO_REDRAW;
    }
}

void Layer_base::quadSetup()
{
    mask_shader = Shader_lib::get_mask_shader();

    baseQuad.addVertex(glm::vec3(0));
	baseQuad.addVertex(glm::vec3(0));
	baseQuad.addVertex(glm::vec3(0));
	baseQuad.addVertex(glm::vec3(0));
			
	baseQuad.addTexCoord(glm::vec2(0.0, 0.0));
	baseQuad.addTexCoord(glm::vec2(1.0, 0.0));
	baseQuad.addTexCoord(glm::vec2(1.0, 1.0));
	baseQuad.addTexCoord(glm::vec2(0.0, 1.0));

    baseQuad.addIndex(0);
    baseQuad.addIndex(1);
    baseQuad.addIndex(2);

    baseQuad.addIndex(2);
    baseQuad.addIndex(3);
    baseQuad.addIndex(0);
}

void Layer_base::setQuad(const ofTexture & _baseTex) const
{
    baseQuad.setVertex(0, glm::vec3( 0                  , 0                   , 0 ));
	baseQuad.setVertex(1, glm::vec3( _baseTex.getWidth(), 0                   , 0 ));
	baseQuad.setVertex(2, glm::vec3( _baseTex.getWidth(), _baseTex.getHeight(), 0 ));
	baseQuad.setVertex(3, glm::vec3( 0                  , _baseTex.getHeight(), 0 ));
}

void Layer_base::drawMasked() const
{

    setQuad(fbo.getTexture());
    fbo.swap();
    fbo.clear();
    fbo.begin();
    mask_shader->begin();
    
    mask_shader->setUniformTexture("u_imageTex", fbo.getBackTexture(), 0);
    mask_shader->setUniformTexture("u_alphaTex", maskFbo.getTexture(), 1);
    mask_shader->setUniform1i("u_invert", (int)p_invertMask );

    baseQuad.draw();
    mask_shader->end();
    fbo.end();
}

void Layer_base::onMask() const
{
    if (p_mask)
    {
        drawMasked();
    }
}