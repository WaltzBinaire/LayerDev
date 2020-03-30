#include "Preset_layers.h"
#include "ProjectManager.h"
#include "layer.h"

using RESOURCE_TYPE = ProjectResource::RESOURCE_TYPE ;

map<string, std::function<void(bool)>> Presets::getPresetLayers(Layer_Manager * layerManager) {
    map<string, std::function<void(bool)>> presetLayers;

    ProjectManager & projectManager = ProjectManager::getInstance();

    // Add Portrait Layer
    if (projectManager.getResource(RESOURCE_TYPE::TARGET) != nullptr) {
        std::function<void(bool)> func = [=](bool _activate) {
            addPortraitLayer(layerManager, _activate);
        };
        presetLayers.insert_or_assign("Portrait", func);

    }

    // Add Insta Collage
    if (projectManager.getResource(RESOURCE_TYPE::PRECOLLAGE) != nullptr) {
        std::function<void(bool)> func = [=](bool _activate) {
            addInstaCollageLayer(layerManager, _activate);
        };
        presetLayers.insert_or_assign("Instacollage", func);

    }

    // Add Collage Layer
    if (projectManager.getResource(RESOURCE_TYPE::SEGMENTED) != nullptr) {
        std::function<void(bool)> func = [=](bool _activate) {
            addCollageLayer(layerManager, _activate);
        };
        presetLayers.insert_or_assign("Collage", func);
    }

    // Add Manual Collage Layer
    if (projectManager.getResource(RESOURCE_TYPE::SEGMENTED) != nullptr) {
        std::function<void(bool)> func = [=](bool _activate) {
            addSingleCollageLayer(layerManager, _activate);
        };
        presetLayers.insert_or_assign("Single Collage", func);
    }

    // Add AI Collage Layer
    if (projectManager.getResource(RESOURCE_TYPE::COLLAGE) != nullptr) {
        std::function<void(bool)> func = [=](bool _activate) {
            addAICollageLayer(layerManager, _activate);
        };
        presetLayers.insert_or_assign("AI Collage", func);
    }

    // Add Face Swap
    if (projectManager.getResource(RESOURCE_TYPE::FACES) != nullptr) {
        std::function<void(bool)> func = [=](bool _activate) {
            addFaceSwapLayer(layerManager, _activate);
        };
        presetLayers.insert_or_assign("Face Swap", func);
    }

    return presetLayers;
}

void Presets::addPortraitLayer(Layer_Manager * layerManager, bool _activate)
{
    ProjectManager & projectManager = ProjectManager::getInstance();
    const vector<string>&  layer_types = layerManager->getLayerTypes();
    const string layer_name = "Mask Image";


    auto targets = projectManager.getResource(RESOURCE_TYPE::TARGET);
    auto masks = projectManager.getResource(RESOURCE_TYPE::MASKS);

    auto layer_type = find(layer_types.begin(), layer_types.end(), layer_name);
    if (layer_type != layer_types.end()) {
        shared_ptr<Layer_base> layer = layerManager->add_layer(layer_name, _activate);
        auto img_layer = dynamic_pointer_cast<Layer_image_advanced>(layer);

        if (img_layer == nullptr) {
            layerManager->delete_layer(layer);
        }
        else {
            img_layer->setDisplayName("Portrait");
            img_layer->handle_file(targets->getFilePath(0));

            if (masks != nullptr) {
                img_layer->loadBodyMask(masks->getFilePath(0));
            }

            img_layer->params.get("Mask").cast<bool>().set(true);
            layerManager->setKeyLayer(layer);
        }
    }
}

void Presets::addCollageLayer(Layer_Manager * layerManager, bool _activate)
{
    ProjectManager & projectManager = ProjectManager::getInstance();
    const vector<string>&  layer_types = layerManager->getLayerTypes();
    const string layer_name = "Generative Collage";

    auto targets = projectManager.getResource(RESOURCE_TYPE::SEGMENTED);

    auto layer_type = find(layer_types.begin(), layer_types.end(), layer_name);
    if (layer_type != layer_types.end()) {
        auto layer = layerManager->add_layer(layer_name, _activate);
        auto collage_layer = dynamic_pointer_cast<Layer_collage_generative>(layer);

        if (collage_layer == nullptr) {
            layerManager->delete_layer(layer);
            return;
        }
        collage_layer->setDisplayName(layer_name + " (Preset)");
        collage_layer->replace_images(targets->getDirectoryPath());
    }
}

void Presets::addSingleCollageLayer(Layer_Manager * layerManager, bool _activate)
{
    ProjectManager & projectManager = ProjectManager::getInstance();
    const vector<string>&  layer_types = layerManager->getLayerTypes();
    const string layer_name = "Single Collage";

    auto targets = projectManager.getResource(RESOURCE_TYPE::SEGMENTED);

    auto layer_type = find(layer_types.begin(), layer_types.end(), layer_name);
    if (layer_type != layer_types.end()) {
        shared_ptr<Layer_base> layer = layerManager->add_layer(layer_name, _activate);
        auto collage_layer = dynamic_pointer_cast<Layer_collage_single>(layer);

        if (collage_layer == nullptr) {
            layerManager->delete_layer(layer);
            return;
        }
        collage_layer->setDisplayName(layer_name + " (Preset)");
        collage_layer->replace_images(targets->getDirectoryPath());
    }
}

void Presets::addAICollageLayer(Layer_Manager * layerManager, bool _activate)
{
    ProjectManager & projectManager = ProjectManager::getInstance();
    const vector<string>&  layer_types = layerManager->getLayerTypes();
    const string layer_name = "AI Collage";

    auto targets = projectManager.getResource(RESOURCE_TYPE::COLLAGE);

    auto layer_type = find(layer_types.begin(), layer_types.end(), layer_name);
    if (layer_type != layer_types.end()) {
        shared_ptr<Layer_base> layer = layerManager->add_layer(layer_name, _activate);
        auto collage_layer = dynamic_pointer_cast<Layer_file_aiCollage>(layer);

        if (collage_layer == nullptr) {
            layerManager->delete_layer(layer);
            return;
        }
        collage_layer->setDisplayName(layer_name + " (Preset)");
        collage_layer->handle_file(targets->getFilePath(0));
    }
}

void Presets::addFaceSwapLayer(Layer_Manager * layerManager, bool _activate)
{
    ProjectManager & projectManager = ProjectManager::getInstance();
    const vector<string>&  layer_types = layerManager->getLayerTypes();
    const string layer_name = "Face Replace";

    auto targets = projectManager.getResource(RESOURCE_TYPE::FACES);

    auto layer_type = find(layer_types.begin(), layer_types.end(), layer_name);
    if (layer_type != layer_types.end()) {
        shared_ptr<Layer_base> layer = layerManager->add_layer(layer_name, _activate);
        auto face_layer = dynamic_pointer_cast<Layer_alpha_replace_face>(layer);

        if (face_layer == nullptr) {
            layerManager->delete_layer(layer);
            return;
        }
        for (int i = 0; i < targets->size(); i++) {
            face_layer->handle_file(targets->getFilePath(i));
        }
        face_layer->setDisplayName(layer_name + " (Preset)");
        face_layer->set_placing();
    }
}

void Presets::addInstaCollageLayer(Layer_Manager * layerManager, bool _activate)
{
    ProjectManager & projectManager = ProjectManager::getInstance();
    const vector<string>&  layer_types = layerManager->getLayerTypes();
    const string layer_name = "Image";


    auto image = projectManager.getResource(RESOURCE_TYPE::PRECOLLAGE);

    auto layer_type = find(layer_types.begin(), layer_types.end(), layer_name);
    if (layer_type != layer_types.end()) {
        shared_ptr<Layer_base> layer = layerManager->add_layer(layer_name, _activate);
        auto img_layer = dynamic_pointer_cast<Layer_file_image>(layer);

        if (img_layer == nullptr) {
            layerManager->delete_layer(layer);
        }
        else {
            img_layer->setDisplayName("Instacollage");
            img_layer->handle_file(image->getFilePath(0));
            img_layer->setDefaultPositionScale();
            img_layer->setPlacing(false);
        }
    }
}