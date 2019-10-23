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

void Layer_collage_generative::onDrawGui()
{
    Layer_collage::onDrawGui();    
    SingleLayerGui::specialisedDrawGui<Layer_collage_generative>(this); 
}

void Layer_collage_generative::generate_lines()
{
    int w  = floor(size.x / p_number);
    int h  = size.y;

    for (int i = 0; i < p_number; i++) {
        int index = i % image_paths.size();
        ofImage newImage(image_paths[index]);

        float scale = size.x / newImage.getHeight();
        newImage.resize(  scale * newImage.getWidth(), scale * newImage.getHeight());

        int x  = floor(ofRandom(0.0, newImage.getWidth() - w));
        int y  = 0.0;

        newImage.crop(x, y, w, h);

        glm::vec2 pos((i + 0.5) * w, h * 0.5);

        images.push_back( CollageImage(newImage, pos) );
    }
}

void Layer_collage_generative::generate_random()
{
    for (int i = 0; i < p_number; i++) {
        int index = i % image_paths.size();
        ofImage newImage(image_paths[index]);

        glm::vec2 pos = glm::vec2(ofRandom(0.0, size.x), ofRandom(0.0, size.y));

        images.push_back( CollageImage(newImage, pos) );
    }
}
