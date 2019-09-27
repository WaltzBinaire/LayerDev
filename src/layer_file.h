#pragma once
#include "layer_base.h"

class Layer_file : public Static_base
{
public:
    Layer_file(string name, Layer_Manager * _layer_manager) : Static_base(name, _layer_manager) {};

protected:
    virtual void onSetupParams() override ;
    virtual void onActivate()    override ;
    virtual void onDeactivate()  override ;

    virtual vector<string> & get_allowed_exts() = 0;
    virtual void handle_file(string path) = 0;

    void onFileDragEvent(ofDragInfo & _fileInfo);
    void onLoadFolder(bool & _loadFolder);
    void onMousePressed ( ofMouseEventArgs & args);
    void onMouseScrolled( ofMouseEventArgs & args);
    void onMouseMoved   ( ofMouseEventArgs & args);

    ofParameter<bool> p_loadFolder;

    float scale = 1.0;
    glm::vec2 position;

    bool b_placing;
};

