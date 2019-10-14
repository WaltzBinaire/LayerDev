#pragma once
#include "Utils/pingPongFbo.h"
#include "Utils/AutoShader.h"
#include "ProjectManager.h"

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

    void setupParams();

    void draw() const;
    void drawGui();
    void update();

    void saveAs() const;
    void save() const;

    void add_layer(string name, bool _activate = true);
    
    vector<string> get_layer_names();

    void redrawAll();

    void setBackgroundColor(float _backgroundColor[4]);
    float* getBackgroundColor() const  {
        return new float[4] {
            (float)(backgroundColour.r / 255.0),
            (float)(backgroundColour.g / 255.0),
            (float)(backgroundColour.b / 255.0),
            (float)(backgroundColour.a / 255.0)
        };
    };

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
    void setupFbo();
    void clearFbo() const;

    void quadSetup();
    void setQuad(const ofTexture & _baseTex) const;   
    
    void internalSave() const;

    void onWindowResized(ofResizeEventArgs & _args);

    deque<Layer_base*>::iterator findLayer(Layer_base* _layer);
    void setActiveLayer(Layer_base* _layer);
    void delete_layer(Layer_base* _layer);
    void move_layer(Layer_base* _layer, DIRECTION _dir);

    ofColor backgroundColour;

    vector<string> layer_types;

    deque<Layer_base*> layers;
    Layer_base* active_layer;

    shared_ptr<AutoShader> base_shader;
    mutable ofMesh baseQuad;


    // Gui
    LayerGui * gui;
    bool b_mouseOverGui;
    friend class LayerGui; 


    mutable pingPongFbo fbo;
    mutable string savePath;
};

