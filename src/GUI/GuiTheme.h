#pragma once

#include "BaseTheme.h"
#include "ofMain.h"

class GuiTheme: public ofxImGui::BaseTheme
{
public:

    void setup() override;
     
    // Text
    ofColor textColor;

    // Menus
    ofColor menuBackground;
};
