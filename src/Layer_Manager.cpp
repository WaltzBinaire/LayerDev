#include "Layer_Manager.h"
#include "layer_base.h"

Layer_Manager::Layer_Manager()
{
    layer_types = Layer_base::get_layer_names();

    gui.setup();
    setupFbo();
    quadSetup();
    setupParams();
    addListeners();

    ofDisableArbTex();
}

void Layer_Manager::setupParams()
{
    gui.begin();
    gui.setTheme( new GuiTheme());
    gui.end();
}


Layer_Manager::~Layer_Manager()
{    
    removeListeners();
}

void Layer_Manager::add_layer(string name, bool _activate)
{
    if (std::find(layer_types.begin(), layer_types.end(), name) == layer_types.end()) {
        ofLogWarning() << name << " is not an available layer type";
    }

    auto layer = Layer_base::create(name, this);
    layer->setup(fbo.getWidth(), fbo.getHeight());
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
    ofAddListener(ofEvents().windowResized, this, &Layer_Manager::onWindowResized);

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
    ofRemoveListener(ofEvents().windowResized, this, &Layer_Manager::onWindowResized);

    ofRemoveListener(ofEvents().mouseMoved    , this, &Layer_Manager::onMouseMoved   );
    ofRemoveListener(ofEvents().mouseDragged  , this, &Layer_Manager::onMouseDragged );
    ofRemoveListener(ofEvents().mousePressed  , this, &Layer_Manager::onMousePressed );
    ofRemoveListener(ofEvents().mouseReleased , this, &Layer_Manager::onMouseReleased);
    ofRemoveListener(ofEvents().mouseScrolled , this, &Layer_Manager::onMouseScrolled);
    ofRemoveListener(ofEvents().mouseEntered  , this, &Layer_Manager::onMouseEntered );
    ofRemoveListener(ofEvents().mouseExited   , this, &Layer_Manager::onMouseExited  );

}

void Layer_Manager::setupFbo()
{
    fbo.allocate(ofGetWidth(), ofGetHeight());
    clearFbo();    
}

void Layer_Manager::clearFbo() const
{
    fbo.clearAll();
}

void Layer_Manager::onWindowResized(ofResizeEventArgs & _args)
{
    setupFbo();
    for (auto & layer : layers) {
        layer->resize(ofGetWidth(), ofGetHeight());
    }
    redrawAll();
}

void Layer_Manager::setActiveLayer(Layer_base* _layer)
{
    if(active_layer != nullptr) active_layer->deactivate();
    active_layer = _layer;
    active_layer->activate();
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
        _layer->deactivate();
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

    clearFbo();

    fbo.begin();
    ofBackground(backgroundColour);
    bool forceRedraw = false;
    for (auto & layer : layers) {
        
        Static_base * s_layer = dynamic_cast<Static_base *>(layer);
        if (s_layer) {
            forceRedraw |= s_layer->draw();
        }
        else {
            Filter_base * f_layer = dynamic_cast<Filter_base *>(layer);
            if (f_layer) {
                fbo.end();
                fbo.swap();
                fbo.begin();
                forceRedraw |= f_layer->draw(fbo.getBackTexture(), forceRedraw);
            }
        }
    }
    fbo.end();

    fbo.draw(0,0);
}

void Layer_Manager::drawGui()
{
    gui.begin();
    
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save", "Ctrl+S")) { save(); }
            if (ImGui::MenuItem("Save As.."))      { saveAs(); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::BeginMenu("Background"))
            {
                ImGui::ColorEdit4("Background Color", (float*)&backgroundColour);
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Add"))
        {
            for (auto & type : layer_types) {
                if (ImGui::MenuItem(type.c_str())) {
                    add_layer(type, true);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

       
    ofxImGui::Settings mainWindow;


    ImGui::SetNextTreeNodeOpen(true);
    if (active_layer != nullptr) {
        ofxImGui::AddGroup(active_layer->params, mainWindow);
    }

    bool sidebar_active = true;
    ImGuiWindowFlags sidebarFlags = ImGuiWindowFlags_None;
    //sidebarFlags |=  ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
    ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_FirstUseEver);     
    ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Layers", &sidebar_active, sidebarFlags)) {

        for( auto itr = layers.rbegin(); itr != layers.rend(); ++itr){
            Layer_base* layer = (*itr);
            string label = layer->get_display_name();

            if (layer == active_layer) label += "*";

            if (ImGui::Button(label.c_str())) {
                setActiveLayer(layer);
            }
            ImGui::SameLine();
            if (ImGui::Button(("+##" + label).c_str())) {
                move_layer(layer, Layer_Manager::UP);
                break;
            }
            ImGui::SameLine();
            if (ImGui::Button(("-##" + label).c_str())) {
                move_layer(layer, Layer_Manager::DOWN);
                break;
            }
            ImGui::SameLine();
            if (ImGui::Button(("d##" + label).c_str())) {
                delete_layer(layer);
                break;
            }
        }    
        ImGui::End();
    }

    b_mouseOverGui = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

    gui.end();
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
    ofPixels pixels;
    fbo.getTexture().readToPixels(pixels);
    ofImage image(pixels);
    image.save(savePath, OF_IMAGE_QUALITY_BEST);
}

void Layer_Manager::quadSetup()
{
    base_shader = Shader_lib::get_passthrough_shader();

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

void Layer_Manager::setQuad(const ofTexture & _baseTex) const
{
    baseQuad.setVertex(0, glm::vec3( 0                  , 0                   , 0 ));
	baseQuad.setVertex(1, glm::vec3( _baseTex.getWidth(), 0                   , 0 ));
	baseQuad.setVertex(2, glm::vec3( _baseTex.getWidth(), _baseTex.getHeight(), 0 ));
	baseQuad.setVertex(3, glm::vec3( 0                  , _baseTex.getHeight(), 0 ));
}

