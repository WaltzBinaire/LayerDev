#include "SingleLayerGui.h"

using namespace ImGuiHelpers;

void SingleLayerGui::baseDrawGui(Layer_base * layer)
{          
    ofParameter<bool> & disable    = layer->params.get("Disable"    ).cast<bool>();    
    ofParameter<bool> & reset      = layer->params.get("Reset"      ).cast<bool>();
    ofParameter<bool> & redraw     = layer->params.get("Redraw"     ).cast<bool>();
    ofParameter<bool> & loadMask   = layer->params.get("Load Mask"  ).cast<bool>();    
    ofParameter<bool> & clearMask  = layer->params.get("Clear Mask" ).cast<bool>();    
    ofParameter<bool> & editMask   = layer->params.get("Edit Mask"  ).cast<bool>();    
    ofParameter<bool> & showMask   = layer->params.get("Show Mask"  ).cast<bool>();    
    ofParameter<bool> & mask       = layer->params.get("Mask"       ).cast<bool>();
    ofParameter<bool> & invertMask = layer->params.get("Invert Mask").cast<bool>();
    ofParameter<bool> & pause      = layer->params.get("Pause"      ).cast<bool>();

    ImGui::Text(layer->get_display_name().c_str());

    IconToggle(pause,   ICON_MDI_PAUSE, ICON_MDI_PLAY);
    ImGui::SameLine();
    IconButton(reset,   ICON_MDI_SYNC);
    ImGui::SameLine();
    IconToggle(disable, ICON_MDI_EYE, ICON_MDI_EYE_OFF);
    ImGui::SameLine();
    IconButton(redraw,  ICON_MDI_BRUSH);
    ImGui::SameLine();
    IconToggle(mask, ICON_MDI_SELECTION, ICON_MDI_SELECT_OFF);
    ImGui::SameLine();

    static string ArtSaveLabel = ofToString(ICON_MDI_CONTENT_SAVE) + "##ArtworkSave";
    if (ImGui::Button( ArtSaveLabel.c_str() )) layer->saveLayer();

    // Mask

    IconButton(loadMask, ICON_MDI_WALLPAPER);
    ImGui::SameLine();
    IconButton(clearMask, ICON_MDI_BLOCK_HELPER);
    ImGui::SameLine();
    IconToggle(editMask , ICON_MDI_PENCIL, ICON_MDI_PENCIL_OFF);
    ImGui::SameLine();
    IconToggle(invertMask, ICON_MDI_INVERT_COLORS_OFF, ICON_MDI_INVERT_COLORS);
    ImGui::SameLine();
    IconToggle(showMask, ICON_MDI_EYE_OUTLINE, ICON_MDI_EYE_OFF_OUTLINE);
    ImGui::SameLine();

    static string MaskSaveLabel = ofToString(ICON_MDI_CONTENT_SAVE) + "##MaskSave";

}
