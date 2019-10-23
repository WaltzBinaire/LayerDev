#pragma once
#include "ofxImGui.h"
#include "IconsMaterialDesignIcons.h"

namespace ImGuiHelpers {

    void LoadButton(ofParameter<bool> & load);
    void Button(ofParameter<bool> & load);
    void IconButton(ofParameter<bool> & load, const char* label);
    void IconToggle(ofParameter<bool> & var, const char* onLabel, const char* offLabel);
    void AngleSlider(ofParameter<float>& parameter);
    void Slider(ofParameter<float>& parameter);
    void Slider(ofParameter<int>& parameter);
    void ColorPicker(ofParameter<glm::vec4>& parameter);
    void SliderVec2(ofParameter<glm::vec2>& parameter);
    void Dropdown(ofParameter<int>& parameter, std::vector<std::string> labels);

    void Checkbox(ofParameter<bool> & parameter);

}
