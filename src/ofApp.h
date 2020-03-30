#pragma once

#include "ofMain.h"
#include "Layer_Manager.h"
#include "Layers/layer.h"
#include "ofxImGui.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

        Layer_Manager manager;
		
};
