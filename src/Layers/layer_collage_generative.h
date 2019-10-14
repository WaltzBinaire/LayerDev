#pragma once
#include "ofMain.h"
#include "Layers\layer_collage.h"

class Layer_collage_generative : public Layer_collage
{
public:
    enum class MODE { RANDOM, LINES };

    Layer_collage_generative(string name, Layer_Manager * _layer_manager) : Layer_collage(name, _layer_manager) {};

    virtual const string get_display_name() const override { return "Folder Collage"; }

protected:
    
    virtual void onSetupParams() override ;

    void onGenerate(bool & _generate);
    virtual void onDrawGui()     override ;

    void generate_random();
    void generate_lines();


    ofParameter<bool> p_generate;
    ofParameter<int>  p_number;
    ofParameter<int>  p_mode;


};

