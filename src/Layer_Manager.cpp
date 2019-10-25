#include "Layer_Manager.h"
#include "Layers/layer.h"
#include "GUI/LayerGui.h"
#include "Utils/LayerUtils.h"

using RESOURCE_TYPE = ProjectResource::RESOURCE_TYPE ;

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

Layer_base * Layer_Manager::add_layer(string name, bool _activate)
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

    return layer;
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


void Layer_Manager::onProjectLoaded(bool & _val)
{

    specialLayers.clear();
    if (!_val)  return;

    // Add Portrait Layer
    if (projectManager().getResource(RESOURCE_TYPE::TARGET) != nullptr) {
        std::function<void(bool)> func = [=](bool _activate) {
            this->addPortraitLayer(_activate);
        };
        specialLayers.insert_or_assign("Portrait", func);
    }

    // Add Collage Layer
    if (projectManager().getResource(RESOURCE_TYPE::SEGMENTED) != nullptr) {
        std::function<void(bool)> func = [=](bool _activate) {
            this->addCollageLayer(_activate);
        };
        specialLayers.insert_or_assign("Collage", func);
    }
}

void Layer_Manager::addPortraitLayer(bool _activate)
{
    const string layer_name = "Mask Image";

    auto targets = projectManager().getResource(RESOURCE_TYPE::TARGET);
    auto masks   = projectManager().getResource(RESOURCE_TYPE::MASKS);

    auto layer_type = find(layer_types.begin(), layer_types.end(), layer_name);
    if (layer_type != layer_types.end()) {
        Layer_base * layer = add_layer(layer_name, _activate);
        Layer_image_advanced * img_layer = dynamic_cast<Layer_image_advanced *>(layer);

        if (img_layer == nullptr) {
            delete_layer(layer);
        }
        else {
            img_layer->set_display_name("Portrait");
            img_layer->handle_file( targets->getFilePath(0));
            img_layer->loadBodyMask(masks->getFilePath(0));
        }
    }
}
void Layer_Manager::addCollageLayer(bool _activate)
{
    const string layer_name = "Generative Collage";

    auto targets = projectManager().getResource(RESOURCE_TYPE::SEGMENTED);

    auto layer_type = find(layer_types.begin(), layer_types.end(), layer_name);
    if (layer_type != layer_types.end()) {
        Layer_base * layer = add_layer(layer_name, _activate);
        Layer_collage_generative * img_layer = dynamic_cast<Layer_collage_generative *>(layer);

        if (img_layer == nullptr) {
            delete_layer(layer);
            return;
        }

        img_layer->populate_images(targets->getDirectoryPath());
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
    ofAddListener(projectManager().onLoaded, this, &Layer_Manager::onProjectLoaded);
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
    ofRemoveListener(projectManager().onLoaded, this, &Layer_Manager::onProjectLoaded);
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

    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);

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

    if (active_layer != nullptr) {
        active_layer->drawOverlay(canvas.getOverlayFbo());
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
    string newPath;
    if (LayerUtils::saveImageDialogue(newPath)) {
        savePath = newPath;
        LayerUtils::saveImage(savePath, canvas.getPixels());
    }
}

void Layer_Manager::save() const
{
    if (savePath == "") {
        saveAs();
    } 
    else {
        LayerUtils::saveImage(savePath, canvas.getPixels());
    }    
}
