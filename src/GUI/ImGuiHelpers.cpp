#include "ImGuiHelpers.h"

namespace ImGuiHelpers {

    void LoadButton(ofParameter<bool> & load)
    {
        if (ImGui::Button("Load File")) {
            load.set(true);
        };
    }

    void Button(ofParameter<bool> & load)
    {
        if (ImGui::Button(load.getName().c_str())) {
            load.set(true);
        };
    }

    void IconButton(ofParameter<bool>& var, const char * label, const char* tooltip)
    {
        if (ImGui::Button(label)) {
            var.set(true);
        }
        if (ImGui::IsItemHovered() && tooltip != nullptr)
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(tooltip);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    void IconToggle(ofParameter<bool>& var, const char * onLabel, const char * offLabel, const char* tooltip)
    {
        if (var.get()) {
            if (ImGui::Button(offLabel)) {
                var.set(false);
            }
        }
        else {
            if (ImGui::Button(onLabel)) {
                var.set(true);
            }
        }
        if (ImGui::IsItemHovered() && tooltip != nullptr)
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(tooltip);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    //--------------------------------------------------------------
    void AngleSlider(ofParameter<float>& parameter)
    {
	    auto tmpRef = parameter.get();
	    if (ImGui::SliderAngle(parameter.getName().c_str(), &tmpRef, parameter.getMin(), parameter.getMax()))
	    {
		    parameter.set(tmpRef);
	    }
    }

    //--------------------------------------------------------------
    void Slider(ofParameter<float>& parameter)
    {
	    auto tmpRef = parameter.get();
	    if (ImGui::SliderFloat(parameter.getName().c_str(), &tmpRef, parameter.getMin(), parameter.getMax()))
	    {
		    parameter.set(tmpRef);
	    }
    }


    //--------------------------------------------------------------
    void Slider(ofParameter<int>& parameter)
    {
	    auto tmpRef = parameter.get();
	    if (ImGui::SliderInt(parameter.getName().c_str(), &tmpRef, parameter.getMin(), parameter.getMax()))
	    {
		    parameter.set(tmpRef);
	    }
    }

    //--------------------------------------------------------------
    void ColorPicker(ofParameter<glm::vec4>& parameter)
    {
        auto tmpRef = parameter.get();
        if (ImGui::ColorEdit4(parameter.getName().c_str(), &tmpRef.r))
	    {
		    parameter.set(tmpRef);
	    }
    }

    //--------------------------------------------------------------
    void SliderVec2(ofParameter<glm::vec2>& parameter)
    {
	    auto tmpRef = parameter.get();
	    if (ImGui::SliderFloat2(parameter.getName().c_str(), glm::value_ptr(tmpRef), parameter.getMin().x, parameter.getMax().x))
	    {
		    parameter.set(tmpRef);
	    }
    }

    //--------------------------------------------------------------
    void Dropdown(ofParameter<int>& parameter, std::vector<std::string> labels)
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

    void Checkbox(ofParameter<bool>& parameter)
    {
        auto tmpRef = parameter.get();
	    if (ImGui::Checkbox(parameter.getName().c_str(), &tmpRef))
	    {
		    parameter.set(tmpRef);
	    }
    }

}
