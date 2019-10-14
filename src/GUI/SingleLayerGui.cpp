#include "SingleLayerGui.h"
#include "IconsMaterialDesignIcons.h"



void SingleLayerGui::baseDrawGui(Layer_base * layer)
{          
    ofParameter<bool> & disable = layer->params.get("Disable").cast<bool>();    
    ofParameter<bool> & reset   = layer->params.get("Reset").cast<bool>();
    ofParameter<bool> & redraw  = layer->params.get("Redraw").cast<bool>();

    ImGui::Text(layer->get_display_name().c_str());

    if (ImGui::Button(ICON_MDI_SYNC)) {
        reset.set(true);
    }

    ImGui::SameLine();
    if (disable.get()) {
        if (ImGui::Button(ICON_MDI_EYE_OFF)) {
            disable.set(false);
        }
    }
    else {
        if (ImGui::Button(ICON_MDI_EYE)) {
            disable.set(true);
        }
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_MDI_BRUSH)) {     
        redraw.set(true);
    }
}

void SingleLayerGui::LoadButton(ofParameter<bool> & load)
{
    if (ImGui::Button("Load File")) {
        load.set(true);
    };
}
