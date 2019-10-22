#include "LayerGui.h"
#include "IconsMaterialDesignIcons.h"

LayerGui::LayerGui()
{
    ImGui::CreateContext();
    setupFonts();

    theme = new GuiTheme();
    gui.setup();
    gui.setTheme(theme);
}

void LayerGui::setupFonts()
{
    const string font_normal_path = ofFilePath::getAbsolutePath("fonts\\sefeo_ui\\segoeui.ttf", true);
    const string font_bold_path   = ofFilePath::getAbsolutePath("fonts\\sefeo_ui\\segoeuib.ttf" , true);
    const string icons_path       = ofFilePath::getAbsolutePath("fonts\\materialDesign\\materialdesignicons.ttf", true);


    ImGuiIO& io = ImGui::GetIO();
    font_normal = setupFont(io, font_normal_path, icons_path);
    font_bold   = setupFont(io, font_bold_path, icons_path);
}

ImFont * LayerGui::setupFont(ImGuiIO & io, const string & path, const string & icon_path)
{

    ImFont * font = io.Fonts->AddFontFromFileTTF(path.c_str(), TEXT_SIZE);
    if (font == nullptr) {
        ofLogError("LayerGui") << "Font could not be found: " << path;
    }
    else if (!font->IsLoaded()) {
        ofLogError("LayerGui") << "Font could not be loaded: " << path;
    }
    else {
        ofLogVerbose("LayerGui") << "Font loaded: " << path;
    }

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    static const ImWchar icon_ranges[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };

    io.Fonts->AddFontFromFileTTF(icon_path.c_str(), 13.0f, &config, icon_ranges);

    return font;
}


LayerGui::~LayerGui()
{
}

void LayerGui::draw(Layer_Manager * manager) const
{
    gui.begin();
    ImGui::PushFont(font_normal);
    drawMainMenuBar(manager);
    drawActiveLayerMenu(manager);
    drawLayerMenu(manager);
    drawProjectMenu(manager);
    ImGui::PopFont();
    gui.end();
}

void LayerGui::drawLayerMenu(Layer_Manager * manager) const
{
    static int width = 350;

    static ImGuiWindowFlags sidebarFlags = 
        ImGuiWindowFlags_None     |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove   |
        ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos( ImVec2(0    , menuBarHeight)                , ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, (ofGetHeight() - menuBarHeight) * 0.66), ImGuiCond_Always);


    if (ImGui::Begin("Layers", NULL, sidebarFlags)) {

        for (auto itr = manager->layers.rbegin(); itr != manager->layers.rend(); ++itr) {
            Layer_base* layer = (*itr);
            string label = layer->get_display_name();
            string id_label = "##" + label;

            if (layer == manager->active_layer) label += "*";

            if (ImGui::Button(label.c_str(), ImVec2(200, 0))) {
                manager->setActiveLayer(layer);
            }
            ImGui::SameLine();
            if (ImGui::Button((ICON_MDI_ARROW_UP_BOLD + id_label).c_str())) {
                manager->move_layer(layer, Layer_Manager::UP);
                break;
            }
            ImGui::SameLine();
            if (ImGui::Button((ICON_MDI_ARROW_DOWN_BOLD + id_label).c_str())) {
                manager->move_layer(layer, Layer_Manager::DOWN);
                break;
            }
            ImGui::SameLine();
            if (ImGui::Button((ICON_MDI_TRASH_CAN + id_label).c_str())) {
                manager->delete_layer(layer);
                break;
            }
        }
        ImGui::End();
    }
}

void LayerGui::drawProjectMenu(Layer_Manager * manager) const
{
    ProjectManager & projectManager = manager->projectManager();

    if (projectManager.isLoaded()) {
        static int width = 450;

        static ImGuiWindowFlags projectWindowFlags = 
            ImGuiWindowFlags_None     |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove   |
            ImGuiWindowFlags_AlwaysVerticalScrollbar |
            ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowPos( ImVec2(ofGetWidth() -  width, menuBarHeight), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, ofGetHeight() - menuBarHeight), ImGuiCond_Always);

        if (ImGui::Begin("Project", NULL, projectWindowFlags)) {
            using RESOURCE_TYPE = ProjectResource::RESOURCE_TYPE;

            ImGui::Text(projectManager.getName().c_str());

            static int current_resource_type = -1;

            ImGui::PushItemWidth(-1);
            ImGui::ListBox("Resource Folders", &current_resource_type, ProjectResource::resource_name_c, 3);
            ImGui::PopItemWidth();


            auto resource = projectManager.getResource((RESOURCE_TYPE)current_resource_type);

            if (resource != nullptr) {

                ostringstream str;
                str << resource->getNumLoadedFiles() << "/" << resource->getNumFiles();
                ImGui::Text(str.str().c_str());

                const vector<ofTexture> & thumbnails = resource->getThumbnails();

                int numPerLine = floor(ImGui::GetWindowWidth() / THUMBNAIL_SIZE);

                int i = 0;
                for (int i = 0; i < thumbnails.size(); i++)
                {
                    ImTextureID tex_id = getTextureId(thumbnails[i]);
                    ImGui::Image(tex_id, ImVec2(THUMBNAIL_SIZE, THUMBNAIL_SIZE));

                    if (i % numPerLine == numPerLine - 1) continue;
                    ImGui::SameLine();
                }
            }

            ImGui::End();
        }
    }

}


void LayerGui::drawActiveLayerMenu(Layer_Manager * manager) const
{
    Layer_base * layer = manager->active_layer;
    if (layer == nullptr) return;

    static int width = 350;

    static ImGuiWindowFlags sidebarFlags = 
        ImGuiWindowFlags_None     |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove   |
        ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos( ImVec2(0.0 ,  menuBarHeight + (ofGetHeight() - menuBarHeight) * 0.66 ) , ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, (ofGetHeight() - menuBarHeight) * 0.33), ImGuiCond_Always);



    if (ImGui::Begin("Layer Settings", NULL, sidebarFlags)) {
            manager->active_layer->drawGui();
    }
    ImGui::End();
}

void LayerGui::drawMainMenuBar(Layer_Manager * manager) const
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            const char T_OPEN[] = ICON_MDI_FOLDER           " Open Project";
            const char T_SAVE[] = ICON_MDI_CONTENT_SAVE     " Save";
            const char T_SAVE_AS[] = ICON_MDI_CONTENT_SAVE_ALL " Save As...";

            if (ImGui::MenuItem(T_OPEN, "Ctrl+O")) { manager->projectManager().loadProject(); }
            ImGui::Separator();
            if (ImGui::MenuItem(T_SAVE, "Ctrl+S")) { manager->save(); }
            if (ImGui::MenuItem(T_SAVE_AS)) { manager->saveAs(); }
            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Settings"))
        {

            if (ImGui::Button("Canvas Settings")) {
                canvasSettingsInit = true;
                ImGui::OpenPopup("Canvas Settings");
            }

            if (ImGui::BeginPopupModal("Canvas Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                Canvas& canvas = manager->getCanvas();

                // Auto resize
                static bool autoResize;
                static bool tmpaAutoResize = canvas.p_autoResize.get();
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

                if (ImGui::Checkbox("Always fit to Window", &tmpaAutoResize)) {
                    autoResize = tmpaAutoResize;
                }

                // Resizing
                static glm::vec2 size = glm::vec2(canvas.getWidth(), canvas.getHeight());
                static char bufw[6] = ""; 
                static char bufh[6] = "";
                if (canvasSettingsInit) {

                    static string w = ofToString(size.x);
                    static string h = ofToString(size.y);

                    copy(w.begin(), w.end(), bufw);
                    copy(h.begin(), h.end(), bufh);
                }

                if (!tmpaAutoResize) {
                    if( ImGui::InputText("Width ", bufw, 6, ImGuiInputTextFlags_CharsDecimal)) size.x = floor(ofToFloat(bufw));
                    if (ImGui::InputText("Height", bufh, 6, ImGuiInputTextFlags_CharsDecimal)) size.y = floor(ofToFloat(bufh));
                }

                // Background colour
                static ImVec4 tmpColor = canvas.getBackgroundColor();
                ImGui::ColorEdit4("Background", (float*)&tmpColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);



                if (ImGui::Button("OK", ImVec2(120,0))) { 
                    canvas.setBackgroundColor(
                        ofColor(
                            tmpColor.x * 255,
                            tmpColor.y * 255,
                            tmpColor.z * 255,
                            tmpColor.w * 255
                        )
                    );

                    canvas.resize(size);
                    canvas.p_autoResize.set(autoResize);
                    ImGui::CloseCurrentPopup();               
                }

                ImGui::SetItemDefaultFocus();
                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120,0))) { ImGui::CloseCurrentPopup(); }

                canvasSettingsInit = false;
                ImGui::EndPopup();
            }
            ImGui::EndMenu();

        }


        if (ImGui::BeginMenu("Add"))
        {
            for (auto & type : manager->layer_types) {
                if (ImGui::MenuItem(type.c_str())) {
                    manager->add_layer(type, true);
                }
            }
            ImGui::EndMenu();
        }

        menuBarHeight = ImGui::GetWindowSize().y;
        ImGui::EndMainMenuBar();

    }
}
