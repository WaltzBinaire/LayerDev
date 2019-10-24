#include "Layers\layer_collage_generative.h"
#include "GUI/SingleLayerGui.h"

REGISTER_TYPE(Layer_collage_generative, Generative Collage)

void Layer_collage_generative::onSetupParams()
{
    Layer_collage::onSetupParams();

    p_generate.set("Generate", false);
    p_generate.addListener(this, &Layer_collage_generative::onGenerate);

    p_number.set("Number", 10, 1, 100);

    p_mode.set("Mode", (int)MODE::RANDOM, (int)MODE::RANDOM, (int)MODE::LINES);

    params.add(
        p_generate,
        p_mode,
        p_number
    );
}

void Layer_collage_generative::onReset()
{
    if (imageLoader != nullptr) {
        imageLoader->forceStop();
        delete imageLoader;
    }
    Layer_collage::onReset();
}

void Layer_collage_generative::onDestroy()
{
    if (imageLoader != nullptr) {
        imageLoader->forceStop();
        delete imageLoader;
    }
}

void Layer_collage_generative::setupPatch(CollagePatch & _patch, int _idx)
{
    MODE mode = (MODE)p_mode.get();

    switch (mode) {
    case MODE::LINES:
        setupPatchLines(_patch, _idx);
        break;
    case MODE::RANDOM:
        setupPatchRandom(_patch, _idx);
        break;
    }
}

void Layer_collage_generative::setupPatchRandom(CollagePatch & _patch, int _idx)
{
    glm::vec2 center = glm::vec2( ofRandom(0.0, size.x), ofRandom(0.0, size.y));
    float scale = ofRandom(0.5, 1.0);
    float angle = 1.0;

    _patch.setup(center, scale, angle);
}

void Layer_collage_generative::setupPatchLines(CollagePatch & _patch, int _idx)
{
    int w  = floor(size.x / p_number);
    int h  = size.y;
    int x  = floor(ofRandom(0.0, _patch.getImageWidth() - w));
    int y  = 0.0;

    float scale = size.x / _patch.getImageHeight();
    glm::vec2 center((_idx + 0.5) * w, h * 0.5);

    _patch.setup(center, scale, 0.0);

    ofImage & newImage = _patch.getImageReference();

    newImage.resize(  scale * newImage.getWidth(), scale * newImage.getHeight());
    newImage.crop(x, y, w, h);;

}

void Layer_collage_generative::onGenerate(bool & _generate)
{
    if (!_generate) return;
    _generate = false;

    if (image_paths.size() == 0) return;

    std::random_shuffle(image_paths.begin(), image_paths.end());
    loadImages();
    redraw();
}

void Layer_collage_generative::onDrawGui()
{
    Layer_collage::onDrawGui();    
    SingleLayerGui::specialisedDrawGui<Layer_collage_generative>(this); 
}

void Layer_collage_generative::loadImages()
{
    if (imageLoader != nullptr) {
        imageLoader->forceStop();
        delete imageLoader;
    }
    images.clear();
    patchInfo.reset();

    imageLoader = new threadedImageLoader();

    images.reserve(p_number);
    for (int i = 0; i < p_number; i++) {
        int index = i % image_paths.size();

        shared_ptr<CollagePatch> newPatch = make_shared<CollagePatch>();

        images.emplace_back(move(newPatch));
        ofImage & image = images.back()->getImageReference();

        imageLoader->loadFromDisk(image, image_paths[index]);
    }
}