#pragma once
#include "ofMain.h"
#include "Layer_Manager.h"


namespace Presets {

    map<string, std::function<void(bool)>> getPresetLayers(Layer_Manager * layerManager);
    
    void addPortraitLayer     (Layer_Manager * layerManager, bool _activate);
    void addCollageLayer      (Layer_Manager * layerManager, bool _activate);
    void addSingleCollageLayer(Layer_Manager * layerManager, bool _activate);
    void addAICollageLayer    (Layer_Manager * layerManager, bool _activate);
    void addFaceSwapLayer     (Layer_Manager * layerManager, bool _activate);
    void addInstaCollageLayer (Layer_Manager * layerManager, bool _activate);
}