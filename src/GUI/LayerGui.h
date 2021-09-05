#pragma once
#include "ofxImGui.h"
#include "GUI/GuiTheme.h"
#include "GUI/ImGuiHelpers.h"

#include "Layer_Manager.h"
#include "Canvas.h"
#include "Layers/layer_base.h"

#include "Utils/Histogram.h"

#define TEXT_SIZE 20.0

class LayerGui
{
public:
    LayerGui();
    ~LayerGui();

    void draw(Layer_Manager * manager);
    bool mouseOverGui() const { return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow); }

private:
    
    float drawMainMenuBar();
    void  drawActiveLayerMenu(ImVec2 pos, ImVec2 size) ;
    void  drawInfoWindow(ImVec2 pos, ImVec2 size)      ;
    void  drawHistrogram(ImVec2 pos, ImVec2 size)      ;
    void  drawLayerMenu(ImVec2 pos, ImVec2 size)       ;
    void  drawProjectMenu(ImVec2 pos, ImVec2 size)     ;
    
    void setupFonts();
    ImFont * setupFont(ImGuiIO & io, const string & path, const string & icon_path);

    ofxImGui::Gui gui;
    GuiTheme* theme;

    ImFont * font_normal;
    ImFont * font_bold;
    
    Histogram histogram;
    Layer_Manager * manager;

    // Internal
    bool canvasSettingsInit;

};

