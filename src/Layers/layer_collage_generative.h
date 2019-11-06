#pragma once
#include "ofMain.h"
#include "Layers\layer_collage.h"
#include "utils\threadedImageLoader.h"

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


protected:
    
    virtual void onSetupParams() override ;
    virtual void onReset()   override;
    virtual void onDestroy() override;

    virtual void setupPatch(CollagePatch & _patch, int _idx) override;
    void setupPatchRandom(CollagePatch & _patch, int _idx);
    void setupPatchLines(CollagePatch & _patch, int _idx);

    void onGenerate(bool & _generate);
    virtual void onDrawGui()     override ;

    void loadImages();


    ofParameter<bool>  p_generate;
    ofParameter<float> p_scale;
    ofParameter<int>   p_number;
    ofParameter<int>   p_mode;

    threadedImageLoader* imageLoader = nullptr;
};

