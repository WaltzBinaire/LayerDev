#include "layer_collage_generative.h"


REGISTER_TYPE(Layer_collage_generative)

void Layer_collage_generative::onSetupParams()
{
    Layer_collage::onSetupParams();

    p_loadFolder.set("Load", false);
    p_loadFolder.addListener(this, &Layer_collage_generative::onLoadFolder);

    p_generate.set("Generate", false);
    p_generate.addListener(this, &Layer_collage_generative::onGenerate);

    p_number.set("Number", 10, 1, 100);

    p_mode.set("Mode", (int)MODE::RANDOM, (int)MODE::RANDOM, (int)MODE::LINES);

    params.add(
        p_loadFolder,
        p_generate,
        p_mode,
        p_number
    );
}

void Layer_collage_generative::onActivate()
{    
    ofAddListener(ofEvents().fileDragEvent, this, &Layer_collage_generative::onFileDragEvent);
}

void Layer_collage_generative::onDeactivate()
{
    ofRemoveListener(ofEvents().fileDragEvent, this, &Layer_collage_generative::onFileDragEvent);
}

void Layer_collage_generative::onFileDragEvent(ofDragInfo & _fileInfo)
{
    if (_fileInfo.files.size() > 1) ofLogWarning() << "Can't add multiple images.";

    string file_path = _fileInfo.files[0];

    ofFile file(file_path);

    if(file)

    if (!file.exists())          return;
    if (!file.isDirectory() )  return;

    ofDirectory dir(file.path());
    populate_images(dir);

}

void Layer_collage_generative::onLoadFolder(bool & _loadFolder)
{
    if (!_loadFolder) return;
    _loadFolder = false;

    auto result = ofSystemLoadDialog("", true);

    if (result.bSuccess) {
        image_paths.clear();
        ofDirectory dir(result.getPath());

        populate_images(dir);
    }

    p_generate.set(true);

}

void Layer_collage_generative::populate_images(ofDirectory &dir)
{
    for (auto ext : allowed_extensions) dir.allowExt(ext);

    dir.listDir();

    image_paths.clear();
    for (auto file : dir) {
        image_paths.push_back(file.path());
    }

    p_generate.set(true);
}

void Layer_collage_generative::onGenerate(bool & _generate)
{
    if (!_generate) return;
    _generate = false;

    if (image_paths.size() == 0) return;
    images.clear();

    std::random_shuffle(image_paths.begin(), image_paths.end());

    switch ((MODE)p_mode.get())
    {
    case MODE::LINES:
        generate_lines();
        break;
    case MODE::RANDOM:
    default:
        generate_random();
        break;
    }

    redraw();

}

void Layer_collage_generative::generate_random()
{
    int w  = floor(size.x / p_number);
    int h  = size.y;

    for (int i = 0; i < p_number; i++) {
        int index = i % image_paths.size();
        ofImage newImage(image_paths[index]);

        float scale = size.x / newImage.getHeight();
        newImage.resize(  scale * newImage.getWidth(), scale * newImage.getHeight());

        int x        = floor(ofRandom(0.0, newImage.getWidth() - w));
        int y  = 0.0;

        newImage.crop(x, y, w, h);

        glm::vec2 pos((i + 0.5) * w, h * 0.5);

        images.push_back( CollageImage(newImage, pos) );
    }
}

void Layer_collage_generative::generate_lines()
{
    for (int i = 0; i < p_number; i++) {
        int index = i % image_paths.size();
        ofImage newImage(image_paths[index]);

        glm::vec2 pos = glm::vec2(ofRandom(0.0, size.x), ofRandom(0.0, size.y));

        images.push_back( CollageImage(newImage, pos) );
    }
}
