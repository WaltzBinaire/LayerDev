#pragma once
#include "ofMain.h"
#include "Layers\layer_collage.h"

class Layer_collage_generative : public Layer_collage
{
public:
    enum class MODE { RANDOM, LINES };

    static const vector<string> & getModeNames() {
        static const vector<string> modeNames{ "Random", "Lines " };
        return modeNames;
    }

    Layer_collage_generative(string name, int instance, Layer_Manager * _layer_manager) 
        : Layer_collage(name, instance, _layer_manager) {};

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

