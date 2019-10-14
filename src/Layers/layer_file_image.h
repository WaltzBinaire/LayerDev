#pragma once
#include "Layers\layer_file.h"
#include "Utils/LayerUtils.h"

class Layer_file_image : public Layer_file
{
public:
    Layer_file_image(string name, Layer_Manager * _layer_manager) : Layer_file(name, _layer_manager) {};

    virtual const string get_display_name() const override { return "Image"; }

protected:

    virtual void onDraw() const override;
    virtual void onReset() override;

    virtual const vector<string> & get_allowed_exts() { return LayerUtils::img_exts; }
    virtual void handle_file(const string & _path) override;

    ofImage img;

};

