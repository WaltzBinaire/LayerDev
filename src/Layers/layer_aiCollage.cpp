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

    p_mode.set("Mode", (int)MODE::NONE, (int)MODE::NONE, (int)MODE::NONE);
    p_mode.addListener(this, &Layer_file_aiCollage::onModeChanged);

    params.add(
        p_mode,
        m_collage.params
    );
}

void Layer_file_aiCollage::onActivate()
{
    switch (mode) {
    case MODE::EDITING:
        addCollageListeners();  
        break;
    case MODE::PLACING:
        addDefaultListeners();
        break;
    }
}

void Layer_file_aiCollage::onDeactivate()
{
    switch (mode) {
    case MODE::EDITING:
        removeCollageListeners();
        break;
    case MODE::PLACING:
        removeDefaultListeners();
        break;
    }
}

void Layer_file_aiCollage::onRender() const
{
    if (m_collage.isLoaded()) {
         m_collage.render();
    }
}

void Layer_file_aiCollage::onDraw() const
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
    MODE newMode = (MODE)_mode;

    switch (newMode) {
    case MODE::EDITING:
        if (mode == MODE::PLACING) removeDefaultListeners();
        addCollageListeners();  
        break;
    case MODE::PLACING:
        if (mode == MODE::EDITING) removeCollageListeners();
        addDefaultListeners();
        break;
    case MODE::NONE :
        if (mode == MODE::PLACING) removeDefaultListeners();
        if (mode == MODE::EDITING) removeCollageListeners();
        break;
    }

    mode = newMode;
}

void Layer_file_aiCollage::addDefaultListeners()
{
    Layer_file::onActivate();
}

void Layer_file_aiCollage::addCollageListeners()
{
    m_collage.setCustomIntereactionEvents(
        &layer_manager->canvasMousePressed,
        &layer_manager->canvasMouseScrolled,
        &ofEvents().keyPressed
    );

    ofAddListener(layer_manager->canvasMouseMoved, this, &Layer_file_aiCollage::onMouseMovedCollage);
    m_collage.setInteractive(true);
}

void Layer_file_aiCollage::removeDefaultListeners()
{
    Layer_file::onDeactivate();
}

void Layer_file_aiCollage::removeCollageListeners()
{
    ofRemoveListener(layer_manager->canvasMouseMoved, this, &Layer_file_aiCollage::onMouseMovedCollage);
    m_collage.setInteractive(false);
}

void Layer_file_aiCollage::onMouseMovedCollage(ofMouseEventArgs & _args)
{
    ofRectangle collageDrawRect( position - size * 0.5, size.x, size.y );

    if (collageDrawRect.inside(_args.x, _args.y)) {
        glm::vec2 adjustedMousePos = glm::vec2(
            (_args.x - collageDrawRect.x) * getFileWidth()   / size.x,
            (_args.y - collageDrawRect.y) * getFileHeight()  / size.y 
        );
        m_collage.updateCursor(adjustedMousePos);
    }
}

void Layer_file_aiCollage::handle_file(const string & _path)
{
    m_collage.load(_path);
    if (m_collage.isLoaded()) redraw(); 

    Layer_file * img_layer = dynamic_cast<Layer_image_advanced *>(layer_manager->keyLayer);
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
