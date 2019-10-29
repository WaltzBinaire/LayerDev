#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	
    ofGLFWWindowSettings windowSettings;
    windowSettings.setGLVersion(4, 1);
    windowSettings.windowMode = OF_WINDOW;

#ifdef NDEBUG
    windowSettings.title = "EMMA // Editor";
#else
    windowSettings.title = "EMMA // Editor - DEBUG";
#endif
    windowSettings.decorated = true;

    ofCreateWindow(windowSettings);

    ofRunApp(new ofApp());

}
