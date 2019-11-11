#include "LayerGui.h"
#include "Layers\layer_base.h"

LayerGui::LayerGui()
{
    ImGui::CreateContext();
    setupFonts();

    monitor.start();

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

void LayerGui::draw(Layer_Manager * manager)
{
    this->manager = manager;

    histogram.setFboSource(manager->getCanvas().getFbo());

    gui.begin();
    ImGui::PushFont(font_normal);

    float menuBarHeight   = drawMainMenuBar();
    float remainingHeight = (ofGetHeight() - menuBarHeight);

    static float leftWidth     = 350;
    static float rightWidth    = 350;
    float rightPosX     = ofGetWidth() - rightWidth;
    
    // RIGHT SIDE

    ImVec2 pos_L0 (0    , menuBarHeight);
    ImVec2 size_L0(leftWidth, remainingHeight * 0.20);

    ImVec2 pos_L1 = pos_L0 + ImVec2( 0.0,  size_L0.y);
    ImVec2 size_L1(leftWidth, remainingHeight * 0.45);
    
    ImVec2 pos_L2 = pos_L1 + ImVec2( 0.0,  size_L1.y);
    ImVec2 size_L2(leftWidth, remainingHeight * 0.35);

    drawHistrogram     ( pos_L0, size_L0);
    drawLayerMenu      ( pos_L1, size_L1);
    drawActiveLayerMenu( pos_L2, size_L2);

    // LEFT SIDE

    ImVec2 pos_R1 (rightPosX , menuBarHeight);
    ImVec2 size_R1(rightWidth, 200);
    
    ImVec2 pos_R2 = pos_R1 + ImVec2( 0.0,  size_R1.y);
    ImVec2 size_R2(leftWidth, remainingHeight - size_R1.y );

    drawInfoWindow(pos_R1, size_R1);
    drawProjectMenu(pos_R2, size_R2);
    ImGui::PopFont();
    gui.end();
}

//---------------------------------------------------------
// MENU BAR
//---------------------------------------------------------
float LayerGui::drawMainMenuBar()
{
    static float menuBarHeight;
    if (ImGui::BeginMainMenuBar())
    {
        // FILE
        //---------------------------------------------------------
        if (ImGui::BeginMenu("File"))
        {
            const char T_OPEN[]    = ICON_MDI_FOLDER            " Open Project";
            const char T_SAVE[]    = ICON_MDI_CONTENT_SAVE      " Save";
            const char T_SAVE_AS[] = ICON_MDI_CONTENT_SAVE_ALL  " Save As...";
            const char T_EXPORT[]  = ICON_MDI_CONTENT_SAVE_MOVE " Export Layers";

            if (ImGui::MenuItem(T_OPEN, "Ctrl+O")) { manager->projectManager().loadProject(); }
            ImGui::Separator();
            if (ImGui::MenuItem(T_SAVE, "Ctrl+S")) { manager->save();  }
            if (ImGui::MenuItem(T_SAVE_AS)) { manager->saveAs();       }
            if (ImGui::MenuItem(T_EXPORT))  { manager->exportLayers(); }
            ImGui::EndMenu();
        }

        // SETTINGS
        //---------------------------------------------------------
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

                if (ImGui::Checkbox("Always fit to Window", &tmpaAutoResize)) {
                    autoResize = tmpaAutoResize;
                }

                // Resizing
                static glm::vec2 size;
                static char bufw[6] = ""; 
                static char bufh[6] = "";

                if(canvasSettingsInit)  size = glm::vec2(canvas.getWidth(), canvas.getHeight());

                if (!tmpaAutoResize) {
                    auto labels     = canvas.getCanavasSizePresets();
	                auto result     = false;
	                int  iSelected  = 0;
	                if (ImGui::BeginCombo("Preset Sizes", labels[iSelected].first.c_str()))
	                {
		                for (int i = 0; i < labels.size(); ++i)
		                {
			                bool selected = (i == iSelected);
			                if (ImGui::Selectable(labels[i].first.c_str(), selected))
			                {
				                iSelected = i;
				                result = true;
			                }
			                if (selected)
			                {
				                ImGui::SetItemDefaultFocus();
			                }
		                }

		                ImGui::EndCombo();
	                }
	                if (result)
	                {
		                size = labels[iSelected].second;
	                }

                    if (canvasSettingsInit || result) {

                        string w = ofToString(size.x);
                        string h = ofToString(size.y);

                        copy(w.begin(), w.end(), bufw);
                        copy(h.begin(), h.end(), bufh);
                    }

                    if (ImGui::InputText("Width ", bufw, 6, ImGuiInputTextFlags_CharsDecimal)) size.x = floor(ofToFloat(bufw));
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
                    canvas.p_autoResize.set(autoResize);
                    canvas.resize(size);
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

        // LAYERS
        //---------------------------------------------------------
        if (ImGui::BeginMenu("Layers"))
        {
            ProjectManager & projectManager = manager->projectManager();

            if (manager->presetLayers.size() > 0 && projectManager.isLoaded()) {
                if (ImGui::BeginMenu("Presets"))
                {
                    for (auto & specialLayer : manager->presetLayers) {
                        if (ImGui::MenuItem(specialLayer.first.c_str())) {
                            (specialLayer.second)(true);
                        }
                    }
                    ImGui::EndMenu();
                }
            }

            if (ImGui::BeginMenu("Standard"))
            {
                for (auto & type : manager->layer_types) {
                    if (Layer_base::isFilter(type)) continue;
                    if (ImGui::MenuItem(type.c_str())) {
                        manager->add_layer(type, true);
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Filters"))
            {
                for (auto & type : manager->layer_types) {
                    if (!Layer_base::isFilter(type)) continue;
                    if (ImGui::MenuItem(type.c_str())) {
                        manager->add_layer(type, true);
                    }
                }
                ImGui::EndMenu();
            }   

            ImGui::EndMenu();
        }

        // TOOLS
        //---------------------------------------------------------
        if (ImGui::BeginMenu("Tools"))
        {
#ifdef NDEBUG
            ProjectManager & projectManager = ProjectManager::getInstance();
            if (projectManager.isLoaded()) {
                if (ImGui::Button("Face Extractor")) {
                    projectManager.populateFaceFolder();
                    ImGui::OpenPopup("Face Extractor...");
                }
            }

            if (ImGui::BeginPopupModal("Face Extractor...", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                FaceExtractor & faceExtractor = FaceExtractor::getInstance();

                ImGui::ProgressBar(faceExtractor.getProgress(), ImVec2(400, 30));

                if (!faceExtractor.isRunning()) {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
#endif // !NDEBUG
            ImGui::EndMenu();

        }
        menuBarHeight = ImGui::GetWindowSize().y;
        ImGui::EndMainMenuBar();
    }
    return menuBarHeight;
}
//---------------------------------------------------------
// LAYERS MENU
//---------------------------------------------------------
void LayerGui::drawLayerMenu(ImVec2 pos, ImVec2 size)
{
    static ImGuiWindowFlags sidebarFlags = 
        ImGuiWindowFlags_None     |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove   |
        ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos( pos , ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    if (ImGui::Begin("Layers", NULL, sidebarFlags)) {

        for (auto itr = manager->layers.rbegin(); itr != manager->layers.rend(); ++itr) {
            shared_ptr<Layer_base> layer = (*itr);
            string label = layer->get_display_name();
            string id_label = "##" + layer->get_unique_name();

            bool isActive = (layer == manager->active_layer);

            if (isActive) {
                ImGui::PushStyleColor(ImGuiCol_Button       , (ImVec4)ofColor::black);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ofColor::black);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive , (ImVec4)ofColor::black);
            }
            if (ImGui::Button((label + id_label).c_str(), ImVec2(170, 0))) {
                manager->setActiveLayer(layer);
            }


            float perf = max(layer->getUpdateTime(), layer->getDrawTime());
            ofColor perfCol = (ofColor::forestGreen).getLerped(ofColor::indianRed,  ofMap(perf, 0.0, 66.0, 0.0, 1.0, true));

            ImGui::PushStyleColor(ImGuiCol_Button       , (ImVec4)perfCol);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)perfCol);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive , (ImVec4)perfCol);
            ImGui::SameLine();
            string speedometer;
            if (perf < 0.33)      speedometer = ICON_MDI_SPEEDOMETER;
            else if( perf < 0.66) speedometer = ICON_MDI_SPEEDOMETER_MEDIUM;
            else                  speedometer = ICON_MDI_SPEEDOMETER_SLOW;
            ImGui::Button((speedometer + id_label).c_str());
            ImGui::PopStyleColor(3);


            ImGui::SameLine();
            if (ImGui::Button((ICON_MDI_ARROW_UP_BOLD + id_label).c_str())) {
                if (isActive) ImGui::PopStyleColor(3);
                manager->move_layer(layer, Layer_Manager::UP);
                break;
            }
            ImGui::SameLine();
            if (ImGui::Button((ICON_MDI_ARROW_DOWN_BOLD + id_label).c_str())) {
                if (isActive) ImGui::PopStyleColor(3);
                manager->move_layer(layer, Layer_Manager::DOWN);
                break;
            }
            ImGui::SameLine();
            if (ImGui::Button((ICON_MDI_TRASH_CAN + id_label).c_str())) {
                if (isActive) ImGui::PopStyleColor(3);
                manager->delete_layer(layer);
                break;
            }

            if (isActive) ImGui::PopStyleColor(3);
        }
        ImGui::End();
    }
}
//---------------------------------------------------------
// ACTIVE LAYER SETTINGS
//---------------------------------------------------------
void LayerGui::drawActiveLayerMenu(ImVec2 pos, ImVec2 size) 
{
    shared_ptr<Layer_base> layer = manager->active_layer;
    if (layer == nullptr) return;

    static ImGuiWindowFlags sidebarFlags = 
        ImGuiWindowFlags_None     |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove   |
        ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos( pos , ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    if (ImGui::Begin("Layer Settings", NULL, sidebarFlags)) {
            manager->active_layer->drawGui();
    }
    ImGui::End();
}

//---------------------------------------------------------
// INFO
//---------------------------------------------------------
void LayerGui::drawInfoWindow(ImVec2 pos, ImVec2 size)
{
    static ImGuiWindowFlags infoWindowFlags = 
            ImGuiWindowFlags_None     |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove   |
            ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos( pos , ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    if (ImGui::Begin("Info", NULL, infoWindowFlags)) {

        static enum InfoType { PROJECT, CPU, FPS, RAM, CACHE} infoType = InfoType::CPU;
        struct Funcs
        {
            static float QueueGetter(void* data, int i) { 
                return (*((deque<float>*)data))[i]; 
            }
        };        

        if (ImGui::Button("Project")) infoType = InfoType::PROJECT; ImGui::SameLine();
        if (ImGui::Button("CPU"))     infoType = InfoType::CPU;     ImGui::SameLine();
        if (ImGui::Button("FPS"))     infoType = InfoType::FPS;     ImGui::SameLine();
        if (ImGui::Button("RAM"))     infoType = InfoType::RAM;     ImGui::SameLine();
        if (ImGui::Button("Cache"))   infoType = InfoType::CACHE;

        static ImVec2 plotSize(0, 100);
        switch (infoType)
        {
        case CPU:
            ImGui::PlotLines("CPU",   Funcs::QueueGetter, (void*)&(monitor.getProcessCpuBuffer())          , monitor.getBufferSize(), 0, NULL, 0.0, 100, plotSize);
            break;
        case FPS:
            ImGui::PlotLines("FPS",   Funcs::QueueGetter, (void*)&(monitor.getFpsBuffer())                 , monitor.getBufferSize(), 0, NULL, 0.0, 120, plotSize);
            break;
        case RAM:
            ImGui::PlotLines("RAM",   Funcs::QueueGetter, (void*)&(monitor.getProcessPysicalMemoryBuffer()), monitor.getBufferSize(), 0, NULL, 0.0, monitor.getTotalPysicalMemory(), plotSize);
            break;
        case CACHE:
            ImGui::PlotLines("CACHE", Funcs::QueueGetter, (void*)&(monitor.getProcessVirtualMemoryBuffer()), monitor.getBufferSize(), 0, NULL, 0.0, monitor.getTotalVirtualMemory(), plotSize);
            break;
        case PROJECT:
        default:
            Canvas& canvas = manager->getCanvas();
            string canvasDimensions = "Canvas: " + ofToString(canvas.getWidth()) + "x" + ofToString(canvas.getHeight());
            ImGui::Text(canvasDimensions.c_str());
            break;
        }

 
    }
    ImGui::End();

}
//---------------------------------------------------------
// HISTOGRAM
//---------------------------------------------------------
void LayerGui::drawHistrogram(ImVec2 pos, ImVec2 size)
{
    static ImGuiWindowFlags histogramFlags = 
        ImGuiWindowFlags_None     |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove   |
        ImGuiWindowFlags_NoScrollbar  |
        ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos( pos , ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    if (ImGui::Begin("Histogram", NULL, histogramFlags)) {
            ImTextureID tex_id;
            ofTexture & tex = histogram.getTexture();

            if (getTextureId( tex, tex_id) ){
                ImGui::Image(tex_id, ImVec2(FBO_RESOLUTION_X, FBO_RESOLUTION_Y));
            }
    }
    ImGui::End();
}
//---------------------------------------------------------
// PROJECT VIEWER
//---------------------------------------------------------
void LayerGui::drawProjectMenu(ImVec2 pos, ImVec2 size)
{
    ProjectManager & projectManager = manager->projectManager();

    if (projectManager.isLoaded()) {
        static ImGuiWindowFlags projectWindowFlags = 
            ImGuiWindowFlags_None     |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove   |
            ImGuiWindowFlags_NoCollapse;

        ImGui::SetNextWindowPos( pos , ImGuiCond_Always);
        ImGui::SetNextWindowSize(size, ImGuiCond_Always);

        if (ImGui::Begin("Project", NULL, projectWindowFlags)) {
            using RESOURCE_TYPE = ProjectResource::RESOURCE_TYPE;

            if (ImGui::Button("Clear")) projectManager.clear();

            ImGui::Text(projectManager.getName().c_str());

            static int current_resource_type = -1;

            ImGui::PushItemWidth(-1);
            ImGui::ListBox("Resource Folders", &current_resource_type, ProjectResource::resource_name_c, 6);
            ImGui::PopItemWidth();


            auto resource = projectManager.getResource((RESOURCE_TYPE)current_resource_type);

            ImGui::BeginChild("Images##", ImVec2(0, 0), false);
            if (resource != nullptr) {

                //ostringstream str;
                //str << resource->getNumLoadedFiles() << "/" << resource->getNumFiles();
                //ImGui::Text(str.str().c_str());

                //const vector<ofTexture> & thumbnails = resource->getThumbnails();

                //int numPerLine = floor(ImGui::GetWindowWidth() / THUMBNAIL_SIZE);

                //int it = 0;
                //for (int i = 0; i < thumbnails.size(); i++)
                //{
                //    ImTextureID tex_id;
                //    
                //    if (getTextureId(thumbnails[i], tex_id) ){
                //        ImGui::Image(tex_id, ImVec2(THUMBNAIL_SIZE, THUMBNAIL_SIZE));

                //        if (it++ % numPerLine == numPerLine - 1) continue;
                //        ImGui::SameLine();
                //    }
                //}
            }
            ImGui::EndChild();

            ImGui::End();
        }
    }

}

