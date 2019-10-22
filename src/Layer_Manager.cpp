#include "Layer_Manager.h"
#include "Layers/layer_base.h"
#include "GUI/LayerGui.h"


Layer_Manager::Layer_Manager()
{
    layer_types = Layer_base::get_layer_names();
    gui = new LayerGui();

    canvas.setup();
    addListeners();

    ofDisableArbTex();

}

Layer_Manager::~Layer_Manager()
{    
    removeListeners();

    if (gui != nullptr) delete(gui);

    for (auto layer = layers.begin(); layer < layers.end(); layer++) {
        if (*layer != nullptr) delete(*layer);
    }
}

void Layer_Manager::add_layer(string name, bool _activate)
{
    if (std::find(layer_types.begin(), layer_types.end(), name) == layer_types.end()) {
        ofLogWarning() << name << " is not an available layer type";
    }

    auto layer = Layer_base::create(name, this);
    layer->setup(canvas.getWidth(), canvas.getHeight());
    layers.push_back(layer);

    if (_activate) {
        setActiveLayer(layer);
    }
}

vector<string> Layer_Manager::get_layer_names()
{
    return layer_types;
}

void Layer_Manager::redrawAll()
{
    for (auto & layer : layers) {
        layer->redraw();
    }
}


void Layer_Manager::addListeners()
{
    ofAddListener(canvas.canvasResized     , this, &Layer_Manager::onCanvasResized);
    ofAddListener(canvas.backgroundChanged , this, &Layer_Manager::onBackgroundChanged);
    ofAddListener(ofEvents().mouseMoved    , this, &Layer_Manager::onMouseMoved   );
    ofAddListener(ofEvents().mouseDragged  , this, &Layer_Manager::onMouseDragged );
    ofAddListener(ofEvents().mousePressed  , this, &Layer_Manager::onMousePressed );
    ofAddListener(ofEvents().mouseReleased , this, &Layer_Manager::onMouseReleased);
    ofAddListener(ofEvents().mouseScrolled , this, &Layer_Manager::onMouseScrolled);
    ofAddListener(ofEvents().mouseEntered  , this, &Layer_Manager::onMouseEntered );
    ofAddListener(ofEvents().mouseExited   , this, &Layer_Manager::onMouseExited  );
}

void Layer_Manager::removeListeners()
{
    ofRemoveListener(canvas.canvasResized     , this, &Layer_Manager::onCanvasResized);
    ofRemoveListener(canvas.backgroundChanged , this, &Layer_Manager::onBackgroundChanged);
    ofRemoveListener(ofEvents().mouseMoved    , this, &Layer_Manager::onMouseMoved   );
    ofRemoveListener(ofEvents().mouseDragged  , this, &Layer_Manager::onMouseDragged );
    ofRemoveListener(ofEvents().mousePressed  , this, &Layer_Manager::onMousePressed );
    ofRemoveListener(ofEvents().mouseReleased , this, &Layer_Manager::onMouseReleased);
    ofRemoveListener(ofEvents().mouseScrolled , this, &Layer_Manager::onMouseScrolled);
    ofRemoveListener(ofEvents().mouseEntered  , this, &Layer_Manager::onMouseEntered );
    ofRemoveListener(ofEvents().mouseExited   , this, &Layer_Manager::onMouseExited  );
}


void Layer_Manager::onCanvasResized(glm::vec2 & _size)
{    
    for (auto & layer : layers) {
        layer->resize(ofGetWidth(), ofGetHeight());
    }
}
void Layer_Manager::onBackgroundChanged(bool & _var)
{    
    redrawAll();
}

void Layer_Manager::setActiveLayer(Layer_base* _layer)
{
    if(active_layer != nullptr) active_layer->deactivate();
    if (active_layer != _layer) {
        active_layer = _layer;
        active_layer->activate();
    }
    else {
        active_layer = nullptr;
    }

}

deque<Layer_base*>::iterator Layer_Manager::findLayer(Layer_base * _layer)
{
    for (auto layer = layers.begin(); layer < layers.end(); layer++) {
        if (*layer == _layer) return layer;
    }
    return layers.end();
}

void Layer_Manager::delete_layer(Layer_base* _layer)
{
    auto layer_itr = findLayer(_layer);
    if (layer_itr != layers.end()) {
        layers.erase(layer_itr);

        if (active_layer == _layer) active_layer = nullptr;
        _layer->destroy();
        delete _layer;
    }
}

void Layer_Manager::move_layer(Layer_base* _layer, DIRECTION _dir)
{
    if (layers.size() <= 1) return;

    auto layer_itr = findLayer(_layer);
    if (layer_itr != layers.end()) {
        auto layer_itr = findLayer(_layer);

        if (_dir == UP) {
            auto next = layer_itr + 1;
            if (next < layers.end()) {
                std::iter_swap(layer_itr, next);
            }
        }
        else if (_dir == DOWN) {
            
            if ( layer_itr != layers.begin() ) {
                auto next = layer_itr - 1;
                std::iter_swap(layer_itr, next);
            }
        }
    }
}

void Layer_Manager::draw() const
{
    canvas.clear(); 
    bool forceRedraw = false;
    for (auto & layer : layers) {
        
        Static_base * s_layer = dynamic_cast<Static_base *>(layer);
        if (s_layer) {
            forceRedraw |= s_layer->draw(canvas.getFbo());
        }
        else {
            Filter_base * f_layer = dynamic_cast<Filter_base *>(layer);
            if (f_layer) {

                forceRedraw |= f_layer->draw(canvas.getFbo(), forceRedraw);
            }
        }
    }
    canvas.draw();
}

void Layer_Manager::drawGui()
{
    gui->draw(this);
    b_mouseOverGui = gui->mouseOverGui();
}

void Layer_Manager::update()
{
    for (auto & layer : layers) {
        layer->update();
    }
}

void Layer_Manager::saveAs() const
{
    auto result = ofSystemSaveDialog("image", "Save as..." );

    if (result.bSuccess) {
        string ext      = ofFilePath::getFileExt(result.filePath);
        string baseName = ofFilePath::getBaseName(result.filePath);
        string dir      = ofFilePath::getEnclosingDirectory(result.filePath);

        if (ext == "jpg" || ext == "png") {
            savePath = result.filePath;
        }
        else {
            savePath = ofFilePath::join(dir, baseName + ".png");
        }
        internalSave();
    }
}

void Layer_Manager::save() const
{
    if (savePath == "") {
        saveAs();
    } 
    else {
        internalSave();
    }
    
}

void Layer_Manager::internalSave() const
{
    ofImage image(canvas.getPixels());
    image.save(savePath, OF_IMAGE_QUALITY_BEST);
}
