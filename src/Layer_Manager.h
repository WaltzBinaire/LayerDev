#pragma once
#include "Utils/AutoShader.h"
#include "ProjectManager.h"
#include "Canvas.h"

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

    Layer_Manager();
    ~Layer_Manager();

    void draw() const;
    void drawGui();
    void update();

    Canvas & getCanvas() { return canvas; }
    const glm::vec2 & getMousePosition() const { return mousePosition; }
    bool mouseOverGui() const { return b_mouseOverGui; }

    void saveAs()       const;
    void save()         const;
    void exportLayers() const;

    Layer_base *  add_layer(string name, bool _activate = true);
    
    vector<string> get_layer_names();
    void redrawAll();

    map<string, std::function<void(bool)>> specialLayers;
    Layer_base * keyLayer = nullptr;

private:

    void onMouseMoved(ofMouseEventArgs & _args);
    void onMouseDragged(ofMouseEventArgs & _args);
    void onMousePressed(ofMouseEventArgs & _args);
    void onMouseReleased(ofMouseEventArgs & _args);
    void onMouseScrolled(ofMouseEventArgs & _args);
    void onMouseEntered(ofMouseEventArgs & _args);
    void onMouseExited(ofMouseEventArgs & _args);

    void onProjectLoaded(bool & _val);
    void addPortraitLayer(bool _activate);
    void addCollageLayer(bool _activate);
    void addManualCollageLayer(bool _activate);
    void addAICollageLayer(bool _activate);
    void addFaceSwapLayer(bool _activate);

    void addListeners();
    void removeListeners();

    void drawFancyCursor() const;

    void onCanvasResized(glm::vec2 & _size);
    void onBackgroundChanged(bool & _var);

    deque<Layer_base*>::iterator findLayer(Layer_base* _layer);
    void setActiveLayer(Layer_base* _layer);
    void delete_layer(Layer_base* _layer);
    void move_layer(Layer_base* _layer, DIRECTION _dir);

    mutable Canvas canvas;
    glm::vec2 mousePosition;
    vector<string> layer_types;

    deque<Layer_base*> layers;
    Layer_base* active_layer;
    

    // Gui
    LayerGui * gui;
    bool b_mouseOverGui;
    friend class LayerGui; 

    mutable string savePath;
};

