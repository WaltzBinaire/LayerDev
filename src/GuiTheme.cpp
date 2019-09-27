#include "GuiTheme.h"


ofColor getRandomColor()
{
    return ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 255);    
}

void GuiTheme::setup()
{
   primary   = ofColor::green;
   secondary = ofColor::forestGreen;
   tertiary  = ofColor::orangeRed;
   light     = ofColor::lightGrey;
   dark      = ofColor::black;


    ImGuiStyle* style = &ImGui::GetStyle();
    
    style->WindowMinSize     = ImVec2(160, 65);
    style->FramePadding      = ImVec2(4, 2);
    style->ItemSpacing       = ImVec2(6, 5);
    style->ItemInnerSpacing  = ImVec2(4, 5);
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
    style->FrameBorderSize   = 1.0f;
    style->ChildBorderSize   = 0.0f;
    
    
    style->Colors[ImGuiCol_Text]                 = ImVec4(primary  , 1.00f);
    style->Colors[ImGuiCol_TextDisabled]         = ImVec4(primary  , 0.58f);
    style->Colors[ImGuiCol_WindowBg]             = ImVec4(dark     , 0.70f);
    style->Colors[ImGuiCol_ChildWindowBg]        = ImVec4(primary  , 0.58f);
    style->Colors[ImGuiCol_Border]               = ImVec4(tertiary , 1.00f);
    style->Colors[ImGuiCol_BorderShadow]         = ImVec4(tertiary , 0.58f);
    style->Colors[ImGuiCol_FrameBg]              = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered]       = ImVec4(secondary, 0.78f);
    style->Colors[ImGuiCol_FrameBgActive]        = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_TitleBg]              = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(secondary, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive]        = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg]            = ImVec4(secondary, 0.47f);
    style->Colors[ImGuiCol_ScrollbarBg]          = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(secondary, 0.21f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(secondary, 0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_CheckMark]            = ImVec4(secondary, 0.80f);
    style->Colors[ImGuiCol_SliderGrab]           = ImVec4(secondary, 0.14f);
    style->Colors[ImGuiCol_SliderGrabActive]     = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_Button]               = ImVec4(secondary, 0.14f);
    style->Colors[ImGuiCol_ButtonHovered]        = ImVec4(secondary, 0.86f);
    style->Colors[ImGuiCol_ButtonActive]         = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_Header]               = ImVec4(secondary, 0.76f);
    style->Colors[ImGuiCol_HeaderHovered]        = ImVec4(secondary, 0.86f);
    style->Colors[ImGuiCol_HeaderActive]         = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_Column]               = ImVec4(secondary, 0.32f);
    style->Colors[ImGuiCol_ColumnHovered]        = ImVec4(secondary, 0.78f);
    style->Colors[ImGuiCol_ColumnActive]         = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip]           = ImVec4(secondary, 0.04f);
    style->Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(secondary, 0.78f);
    style->Colors[ImGuiCol_ResizeGripActive]     = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_PlotLines]            = ImVec4(secondary, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram]        = ImVec4(secondary, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(secondary, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg]       = ImVec4(secondary, 0.43f);
    style->Colors[ImGuiCol_PopupBg]              = ImVec4(secondary, 0.92f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(secondary, 0.73f);
}
