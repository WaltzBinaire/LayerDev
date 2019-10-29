#pragma once
#include "Layers\layer_base.h"

class Layer_file : public Static_base
{
public:

    Layer_file(string name, int instance, Layer_Manager * _layer_manager) : Static_base(name, instance, _layer_manager) {};
    
    
    virtual void handle_file(const string & _path) = 0;

    glm::vec2 getPosition()   const { return position; }
    glm::vec2 getScreenSize() const { return scale * glm::vec2(getFileWidth(), getFileHeight()); }
    float     getScale()      const { return scale; }

protected:
    virtual void onSetupParams() override ;
    virtual void onActivate()    override ;
    virtual void onDeactivate()  override ;
    virtual void onDrawGui()     override ;

    virtual float getFileWidth()  const = 0;
    virtual float getFileHeight() const = 0;


    virtual const vector<string> & get_allowed_exts() = 0;


    void onFileDragEvent(ofDragInfo & _fileInfo);
    void onLoadFile(bool & _loadFolder);
    void onMousePressed ( ofMouseEventArgs & args);
    void onMouseScrolled( ofMouseEventArgs & args);
    void onMouseMoved   ( ofMouseEventArgs & args);

    ofParameter<bool> p_loadFolder;

    float scale = 1.0;
    glm::vec2 position;

    bool b_placing;
    glm::vec2 clickPosition, initPosition;
};

