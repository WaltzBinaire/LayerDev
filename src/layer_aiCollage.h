#pragma once
#include "layer_file.h"
#include "ofxCollageViewer.h"

class Layer_file_aiCollage : public Layer_file
{
public:
    Layer_file_aiCollage(string name, Layer_Manager * _layer_manager) : Layer_file(name, _layer_manager) {};

    virtual const string get_display_name() const override { return "Collage"; }

protected:
    virtual void onSetupParams() override ;
    virtual void onDraw() const override;    
    virtual void onUpdate() override;
    virtual void onReset()  override;

    virtual vector<string> & get_allowed_exts() { return allowed_exts; }
    virtual void handle_file(string path);

private:
    static vector<string> allowed_exts;
    Collage m_collage;
};
