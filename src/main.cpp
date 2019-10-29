#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	
    ofGLFWWindowSettings windowSettings;
    windowSettings.setGLVersion(4, 1);
    windowSettings.windowMode = OF_WINDOW;
    windowSettings.title = "EMMA // Editor";
    windowSettings.decorated = true;

    ofCreateWindow(windowSettings);

    ofRunApp(new ofApp());

}
