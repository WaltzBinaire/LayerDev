#pragma once
#include "ofMain.h"
#include "ProjectManager.h"

namespace LayerUtils {

    template <class Caller>
    bool loadFileDialogue(const vector<string> & allowed_exts, Caller * caller, void (Caller::*_handle_file)(const string&))
    {
        ProjectManager & projectManager = ProjectManager::getInstance();
        
        string defaultPath = "";
        if (projectManager.isLoaded()) defaultPath = projectManager.getPath();

        auto result = ofSystemLoadDialog("Load File", false, defaultPath );

        if (result.bSuccess) {
            ofFile file(result.getPath());

            if (!file.exists()) {
                ofLogWarning(__FUNCTION__) << "Cannot load: " << file.path();
                return false;
            }

            if (std::find(allowed_exts.begin(), allowed_exts.end(), file.getExtension()) == allowed_exts.end()) {
                ofLogWarning(__FUNCTION__) << "Cannot load: " << file.path() << "\nWrong type";
                return false;
            }
            ofLogVerbose(__FUNCTION__) << "Handling " << file.path();
            (*caller.*_handle_file)(file.path());
        }
        return true;
    }
    
    template <class Caller>
    bool loadFolderDialogue(Caller * caller, void (Caller::*_handle_folder)(const string&))
    {

        ProjectManager & projectManager = ProjectManager::getInstance();
        
        string defaultPath = "";
        if (projectManager.isLoaded()) defaultPath = projectManager.getPath();

        auto result = ofSystemLoadDialog("Open Folder...", true, defaultPath );

        if (result.bSuccess) {
            ofDirectory folder(result.getPath());

            if (!folder.exists()) {
                ofLogWarning(__FUNCTION__) << "Cannot open: " << folder.path();
                return false;
            }

            ofLogVerbose(__FUNCTION__) << "Handlinga " << folder.path();
            (*caller.*_handle_folder)(folder.path());
        }
        return true;
    }

    static const vector<string> img_exts  {
            "jpg",
            "JPG",
            "jpeg",
            "JPEG",
            "png",
            "PNG",
            "gif",
            "GIF"
    };

    static const vector<string> collage_exts  { "svg", "SVG" };

}