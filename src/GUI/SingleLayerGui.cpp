#include "SingleLayerGui.h"
#include "IconsMaterialDesignIcons.h"



void SingleLayerGui::baseDrawGui(Layer_base * layer)
{          
    ofParameter<bool> & disable    = layer->params.get("Disable"    ).cast<bool>();    
    ofParameter<bool> & reset      = layer->params.get("Reset"      ).cast<bool>();
    ofParameter<bool> & redraw     = layer->params.get("Redraw"     ).cast<bool>();
    ofParameter<bool> & loadMask   = layer->params.get("Load Mask"  ).cast<bool>();    
    ofParameter<bool> & mask       = layer->params.get("Mask"       ).cast<bool>();
    ofParameter<bool> & invertMask = layer->params.get("Invert Mask").cast<bool>();

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

    // Mask

    if (ImGui::Button(ICON_MDI_WALLPAPER)) {     
        loadMask.set(true);
    }
    ImGui::SameLine();
    if (mask.get()) {
        if (ImGui::Button(ICON_MDI_SELECTION)) {
            mask.set(false);
        }
    }
    else {
        if (ImGui::Button(ICON_MDI_SELECT_OFF)) {
            mask.set(true);
        }
    }
    ImGui::SameLine();
    if (invertMask.get()) {
        if (ImGui::Button(ICON_MDI_INVERT_COLORS_OFF)) {
            invertMask.set(false);
        }
    }
    else {
        if (ImGui::Button(ICON_MDI_INVERT_COLORS)) {
            invertMask.set(true);
        }
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


//--------------------------------------------------------------
void SingleLayerGui::Slider(ofParameter<int>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderInt(parameter.getName().c_str(), &tmpRef, parameter.getMin(), parameter.getMax()))
	{
		parameter.set(tmpRef);
	}
}

void SingleLayerGui::ColorPicker(ofParameter<glm::vec4>& parameter)
{
    auto tmpRef = parameter.get();
    if (ImGui::ColorEdit4(parameter.getName().c_str(), &tmpRef.r))
	{
		parameter.set(tmpRef);
	}
}


//--------------------------------------------------------------
void SingleLayerGui::SliderVec2(ofParameter<glm::vec2>& parameter)
{
	auto tmpRef = parameter.get();
	if (ImGui::SliderFloat2(parameter.getName().c_str(), glm::value_ptr(tmpRef), parameter.getMin().x, parameter.getMax().x))
	{
		parameter.set(tmpRef);
	}
}

//--------------------------------------------------------------
void SingleLayerGui::Dropdown(ofParameter<int>& parameter, std::vector<std::string> labels)
{
	auto result = false;
	auto tmpRef = parameter.get();
	if (ImGui::BeginCombo(parameter.getName().c_str(), labels.at(parameter.get()).c_str()))
	{
		for (int i = 0; i < labels.size(); ++i)
		{
			bool selected = (i == tmpRef);
			if (ImGui::Selectable(labels[i].c_str(), selected))
			{
				tmpRef = i;
				result = true;
			}
			if (selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}
	if (result)
	{
		parameter.set(tmpRef);
	}
}