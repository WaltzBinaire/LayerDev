#include "Layers\layer_aiCollage.h"
#include "layer_image_advanced.h"
#include "GUI/SingleLayerGui.h"


REGISTER_TYPE(Layer_file_aiCollage, AI Collage)

vector<string> Layer_file_aiCollage::allowed_exts{
    "svg"
};

void Layer_file_aiCollage::onSetupParams()
{ 
    Layer_file::onSetupParams();

    p_mode.set("Mode", (int)MODE::NONE, (int)MODE::NONE, (int)MODE::PLACING);
    p_mode.addListener(this, &Layer_file_aiCollage::onModeChanged);

    params.add(
        p_mode,
        m_collage.params
    );
}

void Layer_file_aiCollage::onSetupListeners()
{
    setListeners();
}

void Layer_file_aiCollage::onDeactivate()
{
    p_mode.set((int)MODE::NONE);
}

void Layer_file_aiCollage::onRender(bool _forced) const
{
    if (m_collage.isLoaded()) {
         m_collage.render(false, scale);
    }
}

void Layer_file_aiCollage::onDraw(bool _forced) const
{
    if (m_collage.isLoaded()) {
        glm::vec2 size = scale * glm::vec2(m_collage.getWidth(), m_collage.getHeight());
        m_collage.draw(position - size * 0.5, size.x, size.y);
    }
}

void Layer_file_aiCollage::onDrawOverlay()
{    if (m_collage.isLoaded()) {
        glm::vec2 size = scale * glm::vec2(m_collage.getWidth(), m_collage.getHeight());
        m_collage.drawOverlay(position - size * 0.5, size.x, size.y);
    }
}

void Layer_file_aiCollage::onDrawGui()
{
    Layer_file::onDrawGui();    
    SingleLayerGui::specialisedDrawGui<Layer_file_aiCollage>(this); 
}

void Layer_file_aiCollage::onUpdate()
{
    if (m_collage.needRedraw()) redraw();
}

void Layer_file_aiCollage::onReset()
{
    if (m_collage.isLoaded()) m_collage.clear();
}

void Layer_file_aiCollage::onModeChanged(int & _mode)
{
    mode = (MODE)_mode;
    setListeners();
}

void Layer_file_aiCollage::addCollageListeners()
{
    m_collage.setCustomIntereactionEvents(
        &layer_manager->canvasMousePressed,
        &layer_manager->canvasMouseScrolled,
        &ofEvents().keyPressed
    );

    l_onMouseMoved = layer_manager->canvasMouseMoved.newListener( this, &Layer_file_aiCollage::onMouseMovedCollage);
    m_collage.setInteractive(true);
}

void Layer_file_aiCollage::setListeners()
{
    clearListeners();
    m_collage.setInteractive(false);
    switch (mode) {
    case MODE::EDITING:
        addCollageListeners();  
        break;
    case MODE::PLACING:
        Layer_file::onSetupListeners();
        break;
    case MODE::NONE:
        break;
    }
}

void Layer_file_aiCollage::onMouseMovedCollage(ofMouseEventArgs & _args)
{
    glm::vec2 size = scale * glm::vec2(m_collage.getWidth(), m_collage.getHeight());
    glm::vec2 adjustedMousePos = glm::vec2(
        ((_args.x - (position.x - size.x * 0.5))  / scale),
        ((_args.y - (position.y - size.y * 0.5))  / scale) 
    );
    m_collage.updateCursor(adjustedMousePos);
}

void Layer_file_aiCollage::handle_file(const string & _path)
{
    m_collage.load(_path);
    if (m_collage.isLoaded()) redraw(); 

    auto img_layer = dynamic_pointer_cast<Layer_image_advanced>(layer_manager->keyLayer);
    if (img_layer != nullptr) {
        position = img_layer->getPosition();

        glm::vec2 _scale = img_layer->getScreenSize() / glm::vec2(m_collage.getWidth(),m_collage.getHeight());
        
        if (_scale.x != _scale.y) ofLogWarning(__FUNCTION__) << "Collage does not match portrait aspect ratio";
        scale = _scale.x;
    }
    else {
        p_mode = (int)MODE::PLACING;
    }
}
