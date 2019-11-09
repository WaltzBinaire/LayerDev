#pragma once
#include "Layers/layer.h"
#include "ofxImGui.h"
#include "ImGuiHelpers.h"

using namespace ImGuiHelpers;


class SingleLayerGui
{
public:
    template<class T, enable_if_t<is_base_of<Layer_base,T>::value>* = nullptr> 
    static void drawGui(T * layer);


    template<class T> static void specialisedDrawGui( T * layer);

    template<> static void specialisedDrawGui( Layer_file * layer);
    template<> static void specialisedDrawGui( Layer_image_advanced * layer);
    template<> static void specialisedDrawGui( Layer_collage * layer);
    template<> static void specialisedDrawGui( Layer_collage_generative * layer);
    template<> static void specialisedDrawGui( Layer_file_aiCollage * layer);

    template<> static void specialisedDrawGui( Layer_filter_chromatic_aberation * layer);
    template<> static void specialisedDrawGui( Layer_alpha_replace_face         * layer);
    template<> static void specialisedDrawGui( Layer_filter_alpha_replace       * layer);
    template<> static void specialisedDrawGui( Layer_alpha_replace_channel      * layer);
    template<> static void specialisedDrawGui( Layer_filter_mpeg_glitch         * layer);
    template<> static void specialisedDrawGui( Layer_filter_adjustment          * layer);
    template<> static void specialisedDrawGui( Layer_filter_distort             * layer);
    template<> static void specialisedDrawGui( Layer_paint                      * layer);



private:

    static void baseDrawGui( Layer_base * layer);
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
    ofParameter<bool>  & p_generate  = layer->params.get("Generate").cast<bool>();   
    ofParameter<int>   & p_number    = layer->params.get("Number").cast<int>();  
    ofParameter<int>   & p_mode      = layer->params.get("Mode").cast<int>();  
    ofParameter<float> & p_scale     = layer->params.get("Scale").cast<float>();  

    Dropdown(p_mode, Layer_collage_generative::getModeNames());
    Slider(p_number);
    Slider(p_scale);
    Button(p_generate);
}

template<>
void SingleLayerGui::specialisedDrawGui(Layer_filter_alpha_replace * layer)
{
    ofParameter<bool> & p_load        = layer->params.get("Load").cast<bool>();   
    ofParameter<bool>  & p_hFlip    = layer->params.get("H Flip"    ).cast<bool>();  

    LoadButton(p_load);
    ImGui::SameLine();
    int numLoaded = layer->getNumPaths();
    string infoText = ofToString(numLoaded) + " files loaded";
    ImGui::Text(infoText.c_str());
    Checkbox(p_hFlip  );


}
template<>
void SingleLayerGui::specialisedDrawGui(Layer_alpha_replace_channel * layer)
{  
    ofParameter<bool> & p_invert      = layer->params.get("Invert").cast<bool>(); 
    ofParameter<int>  & p_blurPasses  = layer->params.get("Blur Passes").cast<int>();  
    ofParameter<glm::vec2>& p_thresh      = layer->params.get("Threshold").cast<glm::vec2>();  
    ofParameter<glm::vec4>& p_channelMix = layer->params.get("Channel Mix").cast<glm::vec4>();  

    SliderVec2(p_thresh);
    Slider(p_blurPasses);
    ColorPicker(p_channelMix);    
    Checkbox(p_invert);
}


template<> 
void SingleLayerGui::specialisedDrawGui(Layer_alpha_replace_face * layer) {
    ofParameter<bool>  & p_useMask  = layer->params.get("Mask Face" ).cast<bool>();  
    ofParameter<bool>  & p_lock     = layer->params.get("Lock"      ).cast<bool>();  


    Checkbox(p_useMask); 
    Checkbox(p_lock   );
};

template<>
void SingleLayerGui::specialisedDrawGui(Layer_filter_mpeg_glitch * layer)
{
    ofParameter<float> & p_sizeOfKernel   = layer->params.get("Size of Kernel").cast<float>();   
    ofParameter<float> & p_stopAmount     = layer->params.get("Stop Amount"   ).cast<float>();   
    ofParameter<float> & p_globalStrength = layer->params.get("Strength"      ).cast<float>();   
    ofParameter<bool>  & p_greyScale      = layer->params.get("Greyscale"     ).cast<bool>();  
    ofParameter<bool>  & p_forceUpdate    = layer->params.get("New"           ).cast<bool>();  

    Slider(p_sizeOfKernel  );
    Slider(p_stopAmount    );
    Slider(p_globalStrength);
    Checkbox(p_greyScale);

    Button(p_forceUpdate);
}

template<>
void SingleLayerGui::specialisedDrawGui(Layer_filter_distort * layer)
{
    ofParameter<float> & p_blur   =  layer->params.get("Blur").cast<float>();
    ofParameter<float> & p_size   =  layer->params.get("Size").cast<float>();
    ofParameter<int>   & p_shape  =  layer->params.get("Shape").cast<int>();

    Dropdown(p_shape, Layer_filter_distort::getModeNames());

    Slider(p_blur);
    Slider(p_size);
}


template<>
void SingleLayerGui::specialisedDrawGui(Layer_file_aiCollage * layer)
{

    ofParameter<int> & p_mode = layer->params.get("Mode").cast<int>();

    ofParameterGroup & collageParams = layer->params.getGroup("Collage Settings");

    //ofParameter<float>     & p_areaWeight      = collageParams.get("Area Weight").cast<float>();
    ofParameter<bool>      & p_ShowTarget      = collageParams.get("Show Target").cast<bool>();
    //ofParameter<bool>      & p_ShowBackground  = collageParams.get("Blur").cast<bool>();
    ofParameter<bool>      & p_ShowBase        = collageParams.get("Show Base").cast<bool>();
    ofParameter<bool>      & p_ShowSource      = collageParams.get("Show Source").cast<bool>();
    //ofParameter<bool>      & p_Debug           = collageParams.get("Blur").cast<bool>();
    //ofParameter<bool>      & p_Replace         = collageParams.get("Blur").cast<bool>();
    ofParameter<int>       & p_depth           = collageParams.get("Depth").cast<int>();
    ofParameter<int>       & p_maxPatches      = collageParams.get("Max Patches").cast<int>();
    ofParameter<glm::vec2> & p_alphaRange      = collageParams.get("Alpha").cast<glm::vec2>();

    Dropdown(p_mode, Layer_file_aiCollage::getModeNames());

    if (ImGui::Button(ICON_MDI_SORT)) {
        layer->Sort();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_MDI_SORT_VARIANT)) {         
        layer->SortInternal();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_MDI_SHUFFLE_VARIANT)) {
        layer->Shuffle();
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_MDI_SHUFFLE)) {      
        layer->ShuffleInternal();
    }

    //Checkbox(p_ShowTarget);
    //Checkbox(p_ShowBase);
    //Checkbox(p_ShowSource);

    //Slider(p_depth);
    Slider(p_maxPatches);

    SliderVec2(p_alphaRange);
}


template<> 
static void SingleLayerGui::specialisedDrawGui(Layer_image_advanced * layer) {
    vector<string> masks = layer->getMasks();

    for (auto & mask : masks) {
        string label_id = "##" + mask;
        bool isEnabled = layer->isEnabled(mask);
        Mask::ADD_MODE mode = layer->getMaskMode(mask);

        string name = mask;
        if (ImGui::Button(name.c_str(), ImVec2(200, 0))) { 
            if (!isEnabled) {
                layer->setEnabled(mask, true);
            }
        }

        ImGui::SameLine();
        switch (mode) {
        case Mask::ADD_MODE::AND: 
            if (ImGui::Button((ICON_MDI_GATE_AND + label_id).c_str())) {
                layer->setMaskMode(mask, Mask::ADD_MODE::OR);
            }
            break;
        case Mask::ADD_MODE::OR: 
            if (ImGui::Button((ICON_MDI_GATE_OR + label_id).c_str())) {
                layer->setMaskMode(mask, Mask::ADD_MODE::XOR);
            }
            break;
        case Mask::ADD_MODE::XOR: 
            if (ImGui::Button((ICON_MDI_GATE_XOR + label_id).c_str())) {
                layer->setMaskMode(mask, Mask::ADD_MODE::AND);
            }
            break;
        }

        ImGui::SameLine();
        if (isEnabled) {            
            if (ImGui::Button((ICON_MDI_CHECK + label_id).c_str())) {
                layer->setEnabled(mask, false);
            }
        }
        else {
            if (ImGui::Button((ICON_MDI_BLOCK_HELPER + label_id).c_str())) {
                layer->setEnabled(mask, true);
            }
        }
    }
};


template<> 
static void SingleLayerGui::specialisedDrawGui(Layer_paint * layer) {
    ofParameterGroup   & brushParams = layer->params.getGroup("brush parameters");
    ofParameter<float> & p_scale     = brushParams.get("Scale").cast<float>();
    ofParameter<int>   & p_ColorMode = brushParams.get("Color Mode").cast<int>();

    ofParameterGroup & paintingParams = layer->params.getGroup("painting shader");
    ofParameter<float>  p_normalScale   = paintingParams.get( "Normal Scale"  ).cast<float>();
    ofParameter<float>  p_roughness     = paintingParams.get( "Roughness"     ).cast<float>();
    ofParameter<float>  p_F0            = paintingParams.get( "F0"            ).cast<float>();
    ofParameter<float>  p_diffuseScale  = paintingParams.get( "Diffuse Scale" ).cast<float>();
    ofParameter<float>  p_specularScale = paintingParams.get( "Specular Scale").cast<float>();

    Slider(p_normalScale    );
    Slider(p_roughness      );
    Slider(p_F0             );
    Slider(p_diffuseScale   );
    Slider(p_specularScale  );


};


template<> 
void SingleLayerGui::specialisedDrawGui(Layer_filter_adjustment * layer) {

    ofParameter<float> p_brightness   = layer->params.get( "Brightness"  ).cast<float>();
    ofParameter<float> p_contrast     = layer->params.get( "Contrast"    ).cast<float>();
    ofParameter<float> p_saturation   = layer->params.get( "Saturation"  ).cast<float>();
    ofParameter<float> p_vibrance     = layer->params.get( "Vibrance"    ).cast<float>();
    ofParameter<float> p_hue          = layer->params.get( "Hue"         ).cast<float>();

     Slider(p_brightness );
     Slider(p_contrast   );
     Slider(p_saturation );
     Slider(p_vibrance   );
     Slider(p_hue        );
};