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

    void saveAs() const;
    void save() const;

    void add_layer(string name, bool _activate = true);
    
    vector<string> get_layer_names();

    void redrawAll();

private:

    void onMouseMoved   ( ofMouseEventArgs & _args) { if(!b_mouseOverGui) canvasMouseMoved.notify(this, _args);    }
    void onMouseDragged ( ofMouseEventArgs & _args) { if(!b_mouseOverGui) canvasMouseDragged.notify(this, _args);  }
    void onMousePressed ( ofMouseEventArgs & _args) { if(!b_mouseOverGui) canvasMousePressed.notify(this, _args);  }
    void onMouseReleased( ofMouseEventArgs & _args) { if(!b_mouseOverGui) canvasMouseReleased.notify(this, _args); }
    void onMouseScrolled( ofMouseEventArgs & _args) { if(!b_mouseOverGui) canvasMouseScrolled.notify(this, _args); }
    void onMouseEntered ( ofMouseEventArgs & _args) { if(!b_mouseOverGui) canvasMouseEntered.notify(this, _args);  }
    void onMouseExited  ( ofMouseEventArgs & _args) { if(!b_mouseOverGui) canvasMouseExited.notify(this, _args);   }

    void addListeners();
    void removeListeners();
    
    void internalSave() const;

    void onCanvasResized(glm::vec2 & _size);

    deque<Layer_base*>::iterator findLayer(Layer_base* _layer);
    void setActiveLayer(Layer_base* _layer);
    void delete_layer(Layer_base* _layer);
    void move_layer(Layer_base* _layer, DIRECTION _dir);

    mutable Canvas canvas;
    vector<string> layer_types;

    deque<Layer_base*> layers;
    Layer_base* active_layer;

    // Gui
    LayerGui * gui;
    bool b_mouseOverGui;
    friend class LayerGui; 

    mutable string savePath;
};

