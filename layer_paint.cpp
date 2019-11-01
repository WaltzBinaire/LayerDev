#include "src\Layers\layer_paint.h"

REGISTER_TYPE(Layer_paint, Paint)

void Layer_paint::onRender() const
{
}

void Layer_paint::onDrawGui()
{
}

void Layer_paint::onSetup()
{
}

void Layer_paint::onReset()
{
}

void Layer_paint::onUpdate()
{
}

void Layer_paint::onSetupParams()
{
}

void Layer_paint::onResize()
{
}

void Layer_paint::onSetupListeners()
{
    l_onMousePressed  = layer_manager->canvasMousePressed .newListener( this, &Layer_paint::onMousePressed );
    l_onMouseDragged  = layer_manager->canvasMouseDragged .newListener( this, &Layer_paint::onMouseDragged );
    l_onMouseReleased = layer_manager->canvasMouseReleased.newListener( this, &Layer_paint::onMouseReleased);
    l_onMouseScrolled = layer_manager->canvasMouseScrolled.newListener( this, &Layer_paint::onMouseScrolled);
}

void Layer_paint::setupShader()
{
}

void Layer_paint::onMousePressed(ofMouseEventArgs & _args)
{
}

void Layer_paint::onMouseDragged(ofMouseEventArgs & _args)
{
}

void Layer_paint::onMouseReleased(ofMouseEventArgs & _args)
{
}

void Layer_paint::onMouseScrolled(ofMouseEventArgs & _args)
{
}
