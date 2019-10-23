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
    template<> static void specialisedDrawGui( Layer_alpha_replace_channel * layer);

    template<> static void specialisedDrawGui( Layer_collage * layer);
    template<> static void specialisedDrawGui( Layer_collage_generative * layer);


    template<> static void specialisedDrawGui( Layer_filter_mpeg_glitch * layer);
    template<> static void specialisedDrawGui( Layer_filter_distort * layer);


private:

    static void baseDrawGui( Layer_base * layer);

    static void LoadButton(ofParameter<bool> & load);
    static void Button(ofParameter<bool> & load);
    static void AngleSlider(ofParameter<float>& parameter);
    static void Slider(ofParameter<float>& parameter);
    static void Slider(ofParameter<int>& parameter);
    static void ColorPicker(ofParameter<glm::vec4>& parameter);
    static void SliderVec2(ofParameter<glm::vec2>& parameter);
    static void Dropdown(ofParameter<int>& parameter, std::vector<std::string> labels);

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
    SliderVec2(p_alpha);
}


template<>
void SingleLayerGui::specialisedDrawGui(Layer_collage_generative * layer)
{
    ofParameter<bool> & p_generate  = layer->params.get("Generate").cast<bool>();   
    ofParameter<int>  & p_number    = layer->params.get("Number").cast<int>();  
    ofParameter<int>  & p_mode      = layer->params.get("Mode").cast<int>();  

    Dropdown(p_mode, Layer_collage_generative::getModeNames());
    Slider(p_number);
    Button(p_generate);
}

template<>
void SingleLayerGui::specialisedDrawGui(Layer_alpha_replace_channel * layer)
{
    ofParameter<bool> & p_load        = layer->params.get("Load").cast<bool>();   
    ofParameter<bool> & p_invert      = layer->params.get("Invert").cast<bool>();   
    ofParameter<int>  & p_blurPasses  = layer->params.get("Blur Passes").cast<int>();  
    ofParameter<glm::vec2>& p_thresh      = layer->params.get("Threshold").cast<glm::vec2>();  
    ofParameter<glm::vec4>& p_channelMix = layer->params.get("Channel Mix").cast<glm::vec4>();  

    LoadButton(p_load);
    SliderVec2(p_thresh);
    Slider(p_blurPasses);
    ColorPicker(p_channelMix);
}

template<>
void SingleLayerGui::specialisedDrawGui(Layer_filter_mpeg_glitch * layer)
{
    ofParameter<int> & p_blockSize  = layer->params.get("Block Size").cast<int>();   
    Slider(p_blockSize);
}

template<>
void SingleLayerGui::specialisedDrawGui(Layer_filter_distort * layer)
{

}
