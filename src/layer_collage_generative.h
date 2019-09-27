#pragma once
#include "ofMain.h"
#include "layer_collage.h"

class Layer_collage_generative : public Layer_collage
{
public:
    Layer_collage_generative(string name, Layer_Manager * _layer_manager) : Layer_collage(name, _layer_manager) {};

    virtual const string get_display_name() const override { return "Folder Collage"; }

protected:
    
    virtual void onSetupParams() override ;
    virtual void onActivate()    override ;
    virtual void onDeactivate()  override ;

    void onFileDragEvent(ofDragInfo & _fileInfo);
    void onLoadFolder(bool & _loadFolder);
    void populate_images(ofDirectory &dir);
    void onGenerate(bool & _generate);

    ofParameter<bool> p_loadFolder;
    ofParameter<bool> p_generate;
    ofParameter<int>  p_number;

    vector<string> image_paths;

};

