#pragma once
#include "Layers/layer.h"
#include "ofxImGui.h"

class SingleLayerGui
{
public:
    template<class T, enable_if_t<is_base_of<Layer_base,T>::value>* = nullptr> 
    static void drawGui(T * layer);


    template<class T> static void specialisedDrawGui( T * layer);
    template<> static void specialisedDrawGui( Layer_file * layer);
    template<> static void specialisedDrawGui( Layer_filter_chromatic_aberation * layer);
    template<> static void specialisedDrawGui( Layer_collage * layer);
    template<> static void specialisedDrawGui( Layer_collage_generative * layer);

private:

    static void baseDrawGui( Layer_base * layer);

    static void LoadButton(ofParameter<bool> & load);
    static void Button(ofParameter<bool> & load);
    static void AngleSlider(ofParameter<float>& parameter);
    static void Slider(ofParameter<float>& parameter);

};

template<class T, enable_if_t<is_base_of<Layer_base,T>::value>*> 
void SingleLayerGui::drawGui(T * layer)
{
    baseDrawGui((Layer_base*)layer);
}

template<class T>
void SingleLayerGui::specialisedDrawGui( T * layer)
{
    ofLogWarning(__FUNCTION__) << "No specialised template defined for " << layer->get_display_name();


}

template<>
void SingleLayerGui::specialisedDrawGui(Layer_file * layer)
{
    ofParameter<bool> & p_load = layer->params.get("Load").cast<bool>();  
    LoadButton(p_load);
}


template<>
void SingleLayerGui::specialisedDrawGui(Layer_filter_chromatic_aberation * layer)
{
    ofParameter<float> & p_amount  = layer->params.get("Amount").cast<float>();  
    ofParameter<float> & p_angle   = layer->params.get("Angle").cast<float>();  

    Slider(p_angle);
    Slider(p_amount);
}

template<>
void SingleLayerGui::specialisedDrawGui(Layer_collage * layer)
{
    ofParameter<bool> & p_load = layer->params.get("Load").cast<bool>();  
    ofParameter<glm::vec2> & p_alpha = layer->params.get("Alpha").cast<glm::vec2>();  

    LoadButton(p_load);
}


template<>
void SingleLayerGui::specialisedDrawGui(Layer_collage_generative * layer)
{
    ofParameter<bool> & p_generate       = layer->params.get("Generate").cast<bool>();   
    Button(p_generate);
}
