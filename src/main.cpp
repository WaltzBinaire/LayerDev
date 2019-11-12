#include "ofMain.h"
#include "ofApp.h"
#include "resource.h"

//========================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
	
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

    HWND hwnd = ofGetWin32Window();
    HICON hMyIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MAIN_ICON));
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hMyIcon);

    ofRunApp(new ofApp());

}
