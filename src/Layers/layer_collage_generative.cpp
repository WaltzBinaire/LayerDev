#include "Layers\layer_collage_generative.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_collage_generative, Generative Collage)

void Layer_collage_generative::onSetupParams()
{
    Layer_collage::onSetupParams();

    p_generate.set("Generate", false);
    p_generate.addListener(this, &Layer_collage_generative::onGenerate);

    p_number.set("Number", 40, 1, 500);
    p_scale.set ("Scale" , 4.0, 0.1, 10);

    p_mode.set("Mode", (int)MODE::RANDOM, (int)MODE::RANDOM, (int)MODE::LINES);

    params.add(
        p_generate,
        p_mode,
        p_scale,
        p_number
    );
}

void Layer_collage_generative::setupPatches()
{
    MODE mode = (MODE)p_mode.get();
    patches.clear();    
    patches.reserve(p_number);
    for (int i = 0; i < p_number; i++) {

        string filePath = paths[i % paths.size()];
        patches.emplace_back(make_shared<CollagePatch>(filePath));

        switch (mode) {
        case MODE::LINES:
            setupPatchLines(patches.back(), i);
            break;
        case MODE::RANDOM:
            setupPatchRandom(patches.back(), i);
            break;
        }
    }
}

void Layer_collage_generative::setupPatchRandom(shared_ptr<CollagePatch> _patch, int _idx)
{
    glm::vec2 center = glm::vec2( ofRandom(0.0, size.x), ofRandom(0.0, size.y));
    float scale = ofRandom(0.5 * p_scale, p_scale);
    float angle = 1.0;

    _patch->setup(center, scale, angle);
}

void Layer_collage_generative::setupPatchLines(shared_ptr<CollagePatch> _patch, int _idx)
{
    int w  = floor(size.x / p_number);
    int h  = size.y;
    int x  = floor(ofRandom(0.0, _patch->getImageWidth() - w));
    int y  = 0.0;

    float scale = size.y / _patch->getImageHeight();
    glm::vec2 center((_idx + 0.5) * w, h * 0.5);
    
    _patch->setup(center, scale, 0.0 );
}

void Layer_collage_generative::onGenerate(bool & _generate)
{
    if (!_generate) return;
    _generate = false;

    if (paths.size() == 0) return;

    stopImageLoader();  

    setupCollageFbo();

    std::random_shuffle(paths.begin(), paths.end());
    setupPatches();
    loadAllImages();
}

void Layer_collage_generative::onDrawGui()
{
    Layer_collage::onDrawGui();    
    SingleLayerGui::specialisedDrawGui<Layer_collage_generative>(this); 
}
