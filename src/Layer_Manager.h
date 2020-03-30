#pragma once
#include "Utils/AutoShader.h"
#include "ProjectManager.h"
#include "Canvas.h"
#include "LayerVisualiser.h"

class LayerGui;
class Layer_base;


class Layer_Manager
{
public:
    enum DIRECTION {UP, DOWN};

    ProjectManager & projectManager() { return ProjectManager::getInstance(); };

	ofEvent<ofMouseEventArgs> canvasMouseMoved;
	ofEvent<ofMouseEventArgs> canvasMouseDragged;
	ofEvent<ofMouseEventArgs> canvasMousePressed;
	ofEvent<ofMouseEventArgs> canvasMouseReleased;
	ofEvent<ofMouseEventArgs> canvasMouseScrolled;
	ofEvent<ofMouseEventArgs> canvasMouseEntered;
	ofEvent<ofMouseEventArgs> canvasMouseExited;

    ofEvent<ofKeyEventArgs>   canvasKeyPressed;
	ofEvent<ofKeyEventArgs>   canvasKeyReleased;

    Layer_Manager();
    ~Layer_Manager();

    void draw() const;
    void drawGui();
    void update();

    Canvas & getCanvas() const { return canvas; }
    const glm::vec2 & getMousePosition() const { return mousePosition; }
    bool mouseOverGui() const { return b_mouseOverGui; }

    void saveAs()       const;
    void save()         const;
    void exportLayers() const;

    shared_ptr<Layer_base> add_layer(string name, bool _activate = true);
    deque<shared_ptr<Layer_base>>::iterator findLayer(shared_ptr<Layer_base> _layer);
    void setActiveLayer(shared_ptr<Layer_base> _layer);
    void delete_layer(shared_ptr<Layer_base> _layer);
    void move_layer(shared_ptr<Layer_base> _layer, DIRECTION _dir);
    
    vector<string> get_layer_names();
    const vector<string>&  getLayerTypes() { return layer_types; };

    void setKeyLayer(shared_ptr<Layer_base> _layer);

    size_t getNumLayers() const { return layers.size(); }
    const deque<shared_ptr<Layer_base>> & getLayers() const { return layers; };

    void redrawAll();

    map<string, std::function<void(bool)>> presetLayers;
    shared_ptr<Layer_base> keyLayer;

    LayerVisualiser layerVisualiser;

private:

    void onMouseMoved   (ofMouseEventArgs & _args);
    void onMouseDragged (ofMouseEventArgs & _args);
    void onMousePressed (ofMouseEventArgs & _args);
    void onMouseReleased(ofMouseEventArgs & _args);
    void onMouseScrolled(ofMouseEventArgs & _args);
    void onMouseEntered (ofMouseEventArgs & _args);
    void onMouseExited  (ofMouseEventArgs & _args);

    void onKeyPressed (ofKeyEventArgs & _args);
    void onKeyReleased (ofKeyEventArgs & _args);

    void onProjectLoaded(bool & _val);

    void addListeners();
    void removeListeners();

    void drawFancyCursor() const;

    void onCanvasResized(glm::vec2 & _size);
    void onBackgroundChanged(bool & _var);



    mutable Canvas canvas;
    glm::vec2 mousePosition;
    vector<string> layer_types;

    deque<shared_ptr<Layer_base>> layers;
    shared_ptr<Layer_base> active_layer;
    
    bool b_drawOverlay = true;

    // Gui
    LayerGui * gui;
    bool b_mouseOverGui;
    friend class LayerGui; 

    mutable string savePath;
};

