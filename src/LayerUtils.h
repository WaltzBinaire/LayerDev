#pragma once
#include "ofMain.h"

namespace LayerUtils {

    template <class Caller>
    bool loadFileDialogue(const vector<string> & allowed_exts, Caller * caller, void (Caller::*handle_file)(const string&))
    {
        auto result = ofSystemLoadDialog();

        if (result.bSuccess) {
            ofFile file(result.getPath());

            if (!file.exists()) {
                ofLogWarning(caller->get_name()) << "Cannot load: " << file.path();
                return false;
            }

            if (std::find(allowed_exts.begin(), allowed_exts.end(), file.getExtension()) == allowed_exts.end()) {
                ofLogWarning(caller->get_name()) << "Cannot load: " << file.path() << "\nWrong type";
                return false;
            }
            ofLogVerbose(caller->get_name()) << "Handling " << file.path();
            caller->handle_file(file.path());
        }
        return true;
    }

}