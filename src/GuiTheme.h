#pragma once

#include "BaseTheme.h"
#include "ofMain.h"

class GuiTheme: public ofxImGui::BaseTheme
{
public:

    void setup() override;


    ofColor primary;
    ofColor secondary;
    ofColor tertiary;
    ofColor light;
    ofColor dark;

};
