#include "Layer_Manager.h"
#include "Layers/layer.h"
#include "Preset_layers.h"
#include "GUI/LayerGui.h"
#include "Utils/LayerUtils.h"


using RESOURCE_TYPE = ProjectResource::RESOURCE_TYPE ;

Layer_Manager::Layer_Manager()
{
    layer_types = Layer_base::getLayerNames();
    gui = new LayerGui();

    canvas.setup();
    addListeners();

    ofDisableArbTex();

}

Layer_Manager::~Layer_Manager()
{    
    removeListeners();
    if (gui != nullptr) delete(gui);
}

shared_ptr<Layer_base> Layer_Manager::add_layer(string name, bool _activate)
{
    if (std::find(layer_types.begin(), layer_types.end(), name) == layer_types.end()) {
        ofLogWarning(__FUNCTION__) << name << " is not an available layer type";
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

void Layer_Manager::setKeyLayer(shared_ptr<Layer_base> _layer) {
    auto layer_itr = findLayer(_layer);
    if (layer_itr != layers.end()) {
        keyLayer = _layer;
    }
}

void Layer_Manager::redrawAll()
{
    for (auto & layer : layers) {
        layer->redraw();
    }
}

void Layer_Manager::onMouseMoved(ofMouseEventArgs & _args) { 
    auto args = canvas.transformMouseEventArgs(_args);
    mousePosition = glm::vec2(args.x, args.y);
    if (!b_mouseOverGui) canvasMouseMoved.notify(this, args); 
}

void Layer_Manager::onMouseDragged(ofMouseEventArgs & _args) {
    auto args = canvas.transformMouseEventArgs(_args);
    mousePosition = glm::vec2(args.x, args.y);
    if (!b_mouseOverGui) canvasMouseDragged.notify(this, args); 
}

void Layer_Manager::onMousePressed(ofMouseEventArgs & _args) {
    auto args = canvas.transformMouseEventArgs(_args);
    mousePosition = glm::vec2(args.x, args.y);
    if (!b_mouseOverGui) canvasMousePressed.notify(this, args); 
}

void Layer_Manager::onMouseReleased(ofMouseEventArgs & _args) {
    auto args = canvas.transformMouseEventArgs(_args);
    mousePosition = glm::vec2(args.x, args.y);
    if (!b_mouseOverGui) canvasMouseReleased.notify(this, _args); 
}

void Layer_Manager::onMouseScrolled(ofMouseEventArgs & _args) {
    auto args = canvas.transformMouseEventArgs(_args);
    mousePosition = glm::vec2(args.x, args.y);
    if (!b_mouseOverGui) canvasMouseScrolled.notify(this, args); 
}

void Layer_Manager::onMouseEntered(ofMouseEventArgs & _args) {
    auto args = canvas.transformMouseEventArgs(_args);
    mousePosition = glm::vec2(args.x, args.y);
    if (!b_mouseOverGui) canvasMouseEntered.notify(this, args); 
}

void Layer_Manager::onMouseExited(ofMouseEventArgs & _args) {
    auto args = canvas.transformMouseEventArgs(_args);
    mousePosition = glm::vec2(args.x, args.y);
    if (!b_mouseOverGui) canvasMouseExited.notify(this, args); 
}

void Layer_Manager::onKeyPressed(ofKeyEventArgs & _args)
{

    switch (_args.key) {
    case 'o':
        b_drawOverlay = !b_drawOverlay;
        break;

    }
    canvasKeyPressed.notify(_args);
}

void Layer_Manager::onKeyReleased(ofKeyEventArgs & _args)
{
    canvasKeyReleased.notify(_args);
}


void Layer_Manager::onProjectLoaded(bool & _val)
{
    if (!_val)  return;
    presetLayers = Presets::getPresetLayers(this);
    
    auto potraitLayerFunc = presetLayers.find("Portrait");
    if (potraitLayerFunc != presetLayers.end()) {
        (potraitLayerFunc->second)(true);
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
    ofAddListener(ofEvents().keyPressed    , this, &Layer_Manager::onKeyPressed   );
    ofAddListener(ofEvents().keyReleased   , this, &Layer_Manager::onKeyReleased  );
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
        layer->resize(_size.x, _size.y);
    }
}
void Layer_Manager::onBackgroundChanged(bool & _var)
{    
    redrawAll();
}

void Layer_Manager::setActiveLayer(shared_ptr<Layer_base> _layer)
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

deque<shared_ptr<Layer_base>>::iterator Layer_Manager::findLayer(shared_ptr<Layer_base> _layer)
{
    for (auto layer = layers.begin(); layer < layers.end(); layer++) {
        if (*layer == _layer) return layer;
    }
    return layers.end();
}

void Layer_Manager::delete_layer(shared_ptr<Layer_base> _layer)
{
    auto layer_itr = findLayer(_layer);
    if (layer_itr != layers.end()) {
        layers.erase(layer_itr);

        if (active_layer == _layer) active_layer = nullptr;
    }
}

void Layer_Manager::move_layer(shared_ptr<Layer_base> _layer, DIRECTION _dir)
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
        _layer->redraw();
    }
}

void Layer_Manager::draw() const
{
    bool needsRedraw = false;
    for (auto & layer : layers) {
        needsRedraw |= layer->needsRedraw();
    }
    ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);
    if (needsRedraw) {
        canvas.clearContent(); 
        bool forceRedraw = false;
        for (auto & layer : layers) {
        
            auto s_layer = dynamic_pointer_cast<Static_base>(layer);
            if (s_layer) {
                forceRedraw |= s_layer->draw(canvas.getFbo());
            }
            else {
                auto f_layer = dynamic_pointer_cast<Filter_base>(layer);
                if (f_layer) {

                    forceRedraw |= f_layer->draw(canvas.getFbo(), forceRedraw);
                }
            }
        }
    }
    else if (layers.size() == 0) {
        canvas.clearContent();
    }



    if (ofGetKeyPressed(' ')) {
        layerVisualiser.draw(getLayers());
    }
    else {
        canvas.clearOverlay();
        if (active_layer != nullptr && b_drawOverlay) {
            active_layer->drawOverlay(canvas.getOverlayFbo());
        }
        canvas.draw();

    }




    drawFancyCursor();
}

void Layer_Manager::drawFancyCursor() const
{
    ofPushStyle();
    string str = "x " + ofToString(mousePosition.x) +  "\ny " + ofToString(mousePosition.y) + "\n";

    if (active_layer != nullptr) {
        str += active_layer->getCursorData();
    }

    ofSetColor(ofColor::white);
    ofDrawBitmapString(str, ofGetMouseX() + 30, ofGetMouseY() + 30);
    ofPopStyle();
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

void Layer_Manager::exportLayers() const
{
    string newPath;
    if (LayerUtils::saveFolderDialogue(newPath)) {

        ProjectManager & projectManager = ProjectManager::getInstance();


        string name;
        if (projectManager.isLoaded()) name = projectManager.getName() + "_" + ofGetTimestampString();
        else                           name = "output_" + ofGetTimestampString();

        string folderPath = ofFilePath::join(newPath, name);
        
        ofDirectory dir(folderPath);

        if (!dir.exists()) dir.create();

        string imageName = name + "_Beauty.png";
        string imagePath = ofFilePath::join(folderPath, imageName);
        LayerUtils::saveImage(imagePath, canvas.getPixels());


        int index = 0;
        for (auto & layer : layers) {
            if (layer->isEnabled()) {
                string layerName = name + "_L" + ofToString(index++) +  ".png";
                string layerPath = ofFilePath::join(folderPath, layerName);
                layer->saveLayer(layerPath);
            }

        }
    }
}
