#include "GuiTheme.h"


ofColor getRandomColor()
{
    return ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 255);    
}

void GuiTheme::setup()
{
    textColor      = ofColor(220);
    menuBackground = ofColor::fromHex(0x2d2c31);
    menuLight      = ofColor::fromHex(0x737379);

    ImGuiStyle* style = &ImGui::GetStyle();
    
    style->WindowMinSize     = ImVec2(160, 65);
    style->FramePadding      = ImVec2(10, 6);
    style->ItemSpacing       = ImVec2(10, 6);
    style->ItemInnerSpacing  = ImVec2(10, 6);
    style->Alpha             = 1.0f;
    style->WindowRounding    = 0.0f;
    style->FrameRounding     = 0.0f;
    style->IndentSpacing     = 6.0f;
    style->ItemInnerSpacing  = ImVec2(2, 4);
    style->ColumnsMinSpacing = 50.0f;
    style->GrabMinSize       = 14.0f;
    style->GrabRounding      = 0.0f;
    style->ScrollbarSize     = 12.0f;
    style->ScrollbarRounding = 0.0f;
    style->FrameBorderSize   = 0.0f;
    style->ChildBorderSize   = 0.0f;
    style->ButtonTextAlign   = ImVec2(0.0, 0.5);
    
    
    style->Colors[ImGuiCol_Text]                 = ImVec4(textColor     , 1.00f);
    style->Colors[ImGuiCol_TextDisabled]         = ImVec4(textColor     , 0.80f);
    style->Colors[ImGuiCol_WindowBg]             = ImVec4(menuBackground, 0.80f);
    style->Colors[ImGuiCol_ChildWindowBg]        = ImVec4(menuBackground, 0.80f);
    style->Colors[ImGuiCol_Border]               = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_BorderShadow]         = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_FrameBg]              = ImVec4(menuBackground, 0.80f);
    style->Colors[ImGuiCol_FrameBgHovered]       = ImVec4(menuBackground, 0.80f);
    style->Colors[ImGuiCol_FrameBgActive]        = ImVec4(menuBackground, 0.80f);
    style->Colors[ImGuiCol_TitleBg]              = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive]        = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg]            = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg]          = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(menuLight     , 0.60f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(menuLight     , 0.80f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(menuLight     , 1.00f);
    style->Colors[ImGuiCol_CheckMark]            = ImVec4(menuLight     , 1.00f);
    style->Colors[ImGuiCol_SliderGrab]           = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_SliderGrabActive]     = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_Button]               = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered]        = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_ButtonActive]         = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_Header]               = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered]        = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_HeaderActive]         = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_Column]               = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_ColumnHovered]        = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_ColumnActive]         = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]           = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive]     = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_PlotLines]            = ImVec4(menuLight     , 0.80f);
    style->Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(menuLight     , 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]        = ImVec4(menuLight     , 0.80f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(menuLight     , 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]       = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_PopupBg]              = ImVec4(menuBackground, 1.00f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(menuBackground, 0.10f);
}
