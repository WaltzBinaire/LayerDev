#include "Layers\layer_paint.h"
#include "GUI/SingleLayerGui.h"


REGISTER_TYPE(Layer_paint, Paint)


void Layer_paint::onSetup()
{
    ofEnableArbTex();

    paintingShader = new wbPainting();
    paintingFbo    = new ofFbo();
    brush          = new Brush();
    fluidFlow      = new wbFluidFlow();

    fluidFlow->setSpeed(0.16);
    fluidFlow->setDissipationVel(7.50);
    fluidFlow->setDissipationDen(0.00);
    fluidFlow->setDissipationTmp(0.60);
    fluidFlow->setDissipationPrs(0.85);
    fluidFlow->setViscosityVel (1.0);
    fluidFlow->setViscosityDen (0.0);
    fluidFlow->setViscosityTmp (1.0);
    fluidFlow->setVorticity    (0.0);
    fluidFlow->setBuoyancySigma(0.0);

    b_mouseDown = false;

	densityWidth  = min((int)size.x, MAX_DENSITY_SIZE);
	densityHeight = min((int)size.y, MAX_DENSITY_SIZE);

    paintingFbo->allocate(size.x, size.y, GL_RGBA);

    simulationWidth  = min(densityWidth , MAX_SIMULATION_SIZE);
	simulationHeight = min(densityHeight, MAX_SIMULATION_SIZE);

	fluidFlow->setup(simulationWidth, simulationHeight, densityWidth, densityHeight);
	fluidFlow->setupBrushRenderer(brush->getBristleCount(), brush->getVertsPerBristle(), brush->getSplatsPerSegment());

    brush->init(glm::vec2(ofGetMouseX(), ofGetMouseY()));

    ofDisableArbTex();
}

void Layer_paint::onRender(const ofTexture & _baseTex, bool _forced) const
{
    if (!b_active) return;

    ofEnableArbTex();

    float dt = 1.0 / max(ofGetFrameRate(), 1.f); // more smooth as 'real' deltaTime.
    brush->setColorTexture(_baseTex);

    brush->update( brushPosition );

    if (b_mouseDown) {
        fluidFlow->addDensity (*brush, size);
        fluidFlow->addVelocity(*brush, size);
    }

    
	fluidFlow->update(dt);

    paintingShader->update(*paintingFbo, fluidFlow->getDensity());

    ofDisableArbTex();
}

void Layer_paint::onDraw(const ofTexture & _baseTex, bool _forced) const
{
    ofEnableArbTex();

    _baseTex.draw(0, 0);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    paintingFbo->draw(0, 0, size.x, size.y);


    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofDisableArbTex();
}

void Layer_paint::onDrawOverlay()
{
    brush->drawBrush();  
}

void Layer_paint::onDrawGui()
{
    SingleLayerGui::specialisedDrawGui<Layer_paint>(this); 
}

void Layer_paint::onReset()
{
    fluidFlow->reset(); 
    fluidFlow->resetInput();
    fluidFlow->resetOutput();
}

void Layer_paint::onUpdate()
{
    if(b_active)
        redraw();
}

void Layer_paint::onSetupParams()
{

    params.add(
        brush->params,
        paintingShader->params
    );
}

void Layer_paint::onResize()
{
}

void Layer_paint::onSetupListeners()
{
    l_onMousePressed  = layer_manager->canvasMousePressed .newListener( this, &Layer_paint::onMousePressed );
    l_onMouseDragged  = layer_manager->canvasMouseDragged .newListener( this, &Layer_paint::onMouseDragged );
    l_onMouseMoved    = layer_manager->canvasMouseMoved   .newListener( this, &Layer_paint::onMouseMoved );
    l_onMouseReleased = layer_manager->canvasMouseReleased.newListener( this, &Layer_paint::onMouseReleased);
    l_onMouseScrolled = layer_manager->canvasMouseScrolled.newListener( this, &Layer_paint::onMouseScrolled);
}

void Layer_paint::onMousePressed(ofMouseEventArgs & _args)
{
    b_mouseDown = true;
    brushPosition = glm::vec2(_args.x, _args.y);    
    brush->onMouseDown(_args);
}

void Layer_paint::onMouseDragged(ofMouseEventArgs & _args)
{
    brushPosition = glm::vec2(_args.x, _args.y);
}

void Layer_paint::onMouseMoved(ofMouseEventArgs & _args)
{
    brushPosition = glm::vec2(_args.x, _args.y);
}

void Layer_paint::onMouseReleased(ofMouseEventArgs & _args)
{
    b_mouseDown = false;
}

void Layer_paint::onMouseScrolled(ofMouseEventArgs & _args)
{
    brush->onMouseScrolled(_args);
}
