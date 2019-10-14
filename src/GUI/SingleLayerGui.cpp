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


void SingleLayerGui::Button(ofParameter<bool> & load)
{
    if (ImGui::Button(load.getName().c_str())) {
        load.set(true);
    };
}

//--------------------------------------------------------------
void SingleLayerGui::AngleSlider(ofParameter<float>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderAngle(parameter.getName().c_str(), &tmpRef, parameter.getMin(), parameter.getMax()))
	{
		parameter.set(tmpRef);
	}
}

//--------------------------------------------------------------
void SingleLayerGui::Slider(ofParameter<float>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderFloat(parameter.getName().c_str(), &tmpRef, parameter.getMin(), parameter.getMax()))
	{
		parameter.set(tmpRef);
	}
}
