#pragma once
#include "ofxImGui.h"
#include "GUI/GuiTheme.h"
#include "GUI/ImGuiHelpers.h"

#include "Layer_Manager.h"
#include "Canvas.h"
#include "Layers/layer_base.h"

#define TEXT_SIZE 20.0

class LayerGui
{
public:
    LayerGui();
    ~LayerGui();

    void draw(Layer_Manager * manager) const;
    bool mouseOverGui() const { return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow); }


    static  ImTextureID getTextureId(const ofTexture & _tex) {
        return (ImTextureID)(uintptr_t)(_tex.texData.textureID);
    }

private:
    
    void setupFonts();
    ImFont * setupFont(ImGuiIO & io, const string & path, const string & icon_path);


    void drawMainMenuBar(Layer_Manager * manager)     const;
    void drawActiveLayerMenu(Layer_Manager * manager) const;
    void drawLayerMenu(Layer_Manager * manager)       const;
    void drawProjectMenu(Layer_Manager * manager)     const;
    
    mutable ofxImGui::Gui gui;
    GuiTheme* theme;


    ImFont * font_normal;
    ImFont * font_bold;

    mutable float menuBarHeight;

    bool canvasSettingsInit;

};

