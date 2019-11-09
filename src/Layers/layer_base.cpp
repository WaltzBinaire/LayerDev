#include "Layers\layer_base.h"
#include "GUI\SingleLayerGui.h"
#include "Utils\LayerUtils.h"

Layer_base::Layer_base(string _name, int _instance, Layer_Manager * layer_manager) :
    name(_name),
    instance(_instance),
    b_active(false),
    layer_manager(layer_manager)
{
}

Layer_base::~Layer_base()
{
    onDestroy();
}

void Layer_base::setup(int  _width, int _height) {
    size = glm::vec2(_width, _height);
    setupFbo(size.x, size.y);


    mask_shader    = Shader_lib::get_mask_shader();
    l_onShaderLoad = mask_shader->onLoad.newListener( [&](bool &) {return this->redraw(); });

    onSetup();

    p_reset.addListener(this, &Layer_base::onResetInternal);
    
    p_reset.set("Reset", false);
    p_disable.set("Disable", false);
    p_debugRedraw.set("Redraw", false);  
    p_pause.set("Pause"  , false);

    p_loadMask.addListener (this, &Layer_base::onLoadMask);
    p_clearMask.addListener(this, &Layer_base::onClearMask);
    p_editMask.addListener (this, &Layer_base::onEditMask);

    p_loadMask.set  ( "Load Mask"  , false);
    p_clearMask.set ( "Clear Mask" , false);
    p_editMask.set  ( "Edit Mask"  , false);
    p_showMask.set  ( "Show Mask"  , false);
    p_mask.set      ( "Mask"       , false);
    p_invertMask.set( "Invert Mask", false);

    params.add(
        p_reset,
        p_disable,
        p_debugRedraw,
        p_loadMask,
        p_clearMask,
        p_showMask,
        p_editMask,
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

    if (p_showMask) maskFbo.draw(0, 0);
    if (p_editMask) drawMaskBrush();

    onDrawOverlay();
    overlayFbo.end();
}

void Layer_base::saveLayer()
{
    string newPath;
    if (LayerUtils::saveImageDialogue(newPath)) {
        saveLayer(newPath);
    }
}

void Layer_base::saveLayer(string path)
{
    LayerUtils::saveImage(path, fbo);
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

shared_ptr<Layer_base> Layer_base::create(const string & name, Layer_Manager * _layer_manager )
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

void Layer_base::update() { 
    updatePerfCounter.begin();
    if (!p_pause) time = ofGetSystemTimeMillis();  
    onUpdate(); 
    updatePerfCounter.end();

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
    clearMaskFbo();

}

void Layer_base::resize(int _width, int _height )
{
    ofLogNotice(name) << "Resizing " << _width << "x" << _height;
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

void Layer_base::clearListeners()
{
    l_onMouseMoved.unsubscribe();
    l_onMouseDragged.unsubscribe();
    l_onMousePressed.unsubscribe();
    l_onMouseReleased.unsubscribe();
    l_onMouseScrolled.unsubscribe();
    l_onMouseEntered.unsubscribe();
    l_onMouseExited.unsubscribe();
    l_onFileDragged.unsubscribe();
    l_onKeyPressed.unsubscribe();;
    l_onKeyReleased.unsubscribe();
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

void Layer_base::onEditMask(bool & _val)
{
    clearListeners();
    if (_val) {    
        l_onMousePressed  = layer_manager->canvasMousePressed.newListener ( this,  &Layer_base::onMaskEditMousePressed );
        l_onMouseDragged  = layer_manager->canvasMouseDragged.newListener ( this,  &Layer_base::onMaskEditMouseDragged );
        l_onMouseScrolled = layer_manager->canvasMouseScrolled.newListener( this,  &Layer_base::onMaskEditMouseScrolled );
        l_onMouseMoved    = layer_manager->canvasMouseMoved.newListener   ( this,  &Layer_base::onMaskEditMouseMoved    );

        if (!p_mask || p_showMask) {
            p_mask.set(true);
        }
    }
    else {
        onSetupListeners();
    }
}

void Layer_base::onMaskEditMousePressed(ofMouseEventArgs & _args)
{
    updateMaskBrushPosition(_args);
    drawMaskEditBrush(_args);
}

void Layer_base::onMaskEditMouseMoved(ofMouseEventArgs & _args)
{
    updateMaskBrushPosition(_args);
}

void Layer_base::onMaskEditMouseDragged(ofMouseEventArgs & _args)
{
    updateMaskBrushPosition(_args);
    drawMaskEditBrush(_args);
}

void Layer_base::onMaskEditMouseScrolled(ofMouseEventArgs & _args)
{
    maskBrushSize += 5.0 * _args.scrollY;
    maskBrushSize = ofClamp(maskBrushSize, MIN_MASK_BRUSH_SIZE, MAX_MASK_BRUSH_SIZE);
}

void Layer_base::drawMaskEditBrush(ofMouseEventArgs & _args)
{
    ofPushStyle();
    if (_args.button == 0) ofSetColor(ofColor::black);
    else                   ofSetColor(ofColor::white);

    maskFbo.begin();
    ofDrawRectangle(
        maskBrushPosition.x - maskBrushSize * 0.5,
        maskBrushPosition.y - maskBrushSize * 0.5,
        maskBrushSize,
        maskBrushSize
    );

    maskFbo.end();

    redraw();
    ofPopStyle();
}

void Layer_base::updateMaskBrushPosition(ofMouseEventArgs & _args)
{
    maskBrushPosition = glm::vec2(_args.x, _args.y);
    maskBrushPosition = maskBrushSize * glm::floor((glm::vec2(maskBrushSize * 0.5) + maskBrushPosition) / maskBrushSize);
}

void Layer_base::drawMaskBrush() const
{
    ofPushStyle();

    float edges[4]{
        maskBrushPosition.x - maskBrushSize * 0.5,
        maskBrushPosition.y - maskBrushSize * 0.5,
        maskBrushPosition.x + maskBrushSize * 0.5,
        maskBrushPosition.y + maskBrushSize * 0.5,
    };

    glm::vec2 points[4]{
        {edges[0], edges[1]},
        {edges[2], edges[1]},
        {edges[2], edges[3]},
        {edges[0], edges[3]}
    };

    ofSetColor(ofColor(255, 255, 255, 192));
    ofDrawRectangle(points[0], points[2].x - points[0].x, points[2].y - points[0].y);

    ofSetColor(ofColor::white);
    ofSetLineWidth(8.0);
    ofDrawLine(points[0], points[1]);
    ofDrawLine(points[1], points[2]);
    ofDrawLine(points[2], points[3]);
    ofDrawLine(points[3], points[0]);



    ofPopStyle();
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
    drawPerfCounter.begin();
    bool status;

    if (p_disable) {
        status = needsRedraw();
    }
    else if (needsRedraw() || _forceRedraw) {

        onRender(_forceRedraw);

        fbo.begin();
        ofClear(0);
        onDraw(_forceRedraw);
        fbo.end();
        
        setRedraw(false);

        onMask();

        mainFbo.begin();
        fbo.draw(0, 0);
        mainFbo.end();
        status = REDRAW;

    } else {        

        mainFbo.begin();
        fbo.draw(0, 0);
        mainFbo.end();
        status = NO_REDRAW;
    }

    drawPerfCounter.end();
    return status;
}

bool Filter_base::draw(pingPongFbo & mainFbo, bool _forceRedraw) const
{
    drawPerfCounter.begin();
    bool status;
    if (p_disable ) {
        status = needsRedraw();
    } else if (needsRedraw() || _forceRedraw) {
        mainFbo.swap();

        onRender(mainFbo.getBackTexture(), _forceRedraw);

        fbo.begin();
        ofClear(0);
        onDraw(mainFbo.getBackTexture(), _forceRedraw);
        fbo.end();

        setRedraw(false);

        onMask(mainFbo.getBackTexture());
        
        mainFbo.begin();
        fbo.draw(0, 0);
        mainFbo.end();

        status = REDRAW;
    }
    else {
        mainFbo.begin();
        fbo.draw(0, 0);
        mainFbo.end();
        status = NO_REDRAW;
    }
    
    drawPerfCounter.end();
    return status;

}

void Static_base::onMask() const
{
    if (p_mask)
    {
        fbo.swap();
        fbo.clear();
        fbo.begin();
        mask_shader->begin();
    
        mask_shader->setUniformTexture("u_imageTex", fbo.getBackTexture(), 0);
        mask_shader->setUniformTexture("u_alphaTex", maskFbo.getTexture(), 1);
        mask_shader->setUniform1i("u_invert", (int)p_invertMask );

        LayerUtils::UVQuad::getInstance().draw(0, 0, size.x, size.y); 

        mask_shader->end();
        fbo.end();
    }
}

void Filter_base::onMask(const ofTexture & _baseTex) const
{
    if (p_mask)
    {
        fbo.swap();
        fbo.clear();
        fbo.begin();
        _baseTex.draw(0, 0);
        mask_shader->begin();
    
        mask_shader->setUniformTexture("u_imageTex", fbo.getBackTexture(), 0);
        mask_shader->setUniformTexture("u_alphaTex", maskFbo.getTexture(), 1);
        mask_shader->setUniform1i("u_invert", (int)p_invertMask );

        LayerUtils::UVQuad::getInstance().draw(0, 0, size.x, size.y); 

        mask_shader->end();
        fbo.end();
    }
}
