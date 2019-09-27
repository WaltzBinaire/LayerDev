#include "layer_collage_manual.h"

REGISTER_TYPE(Layer_collage_manual)

void Layer_collage_manual::onActivate()
{
    ofAddListener(ofEvents().fileDragEvent, this, &Layer_collage_manual::onFileDragEvent);
}

//--------------------------------------------------------------
void Layer_collage_manual::onDeactivate()
{
    ofRemoveListener(ofEvents().fileDragEvent, this, &Layer_collage_manual::onFileDragEvent);
}

//--------------------------------------------------------------
void Layer_collage_manual::onFileDragEvent(ofDragInfo & _fileInfo)
{

    glm::vec2 pos = _fileInfo.position;

    if (_fileInfo.files.size() > 1) ofLogWarning() << "Can't add multiple images.";

    string file_path = _fileInfo.files[0];

    ofFile file(file_path);

    ofLogVerbose() << "File dragged " << file.getExtension() ;

    if (!file.exists())          return;
    if (!extensionValid(file) )  return;


    ofImage newImage = ofImage(file.getAbsolutePath());

    images.push_back( CollageImage(newImage, pos) );
    active_image = images.end() - 1;
    setMode(Mode::EDITING);

    ofLogVerbose() << "Added image: " << file.path();
}
//--------------------------------------------------------------
void Layer_collage_manual::onMouseMoved(ofMouseEventArgs & _args)
{
    float angle = glm::angle(
        glm::vec2(ofGetMouseX(), ofGetMouseY()),
        active_image->center
    );
}

//--------------------------------------------------------------
void Layer_collage_manual::onMousePressed(ofMouseEventArgs & _args)
{
    setMode(Mode::VIEWING);
}

//--------------------------------------------------------------
void Layer_collage_manual::onMouseScrolled(ofMouseEventArgs & _args)
{
    active_image->scale += _args.scrollY / 20.0f;
    ofLogNotice() << active_image->scale;
}

//--------------------------------------------------------------
void Layer_collage_manual::onModeViewing()
{
    ofRemoveListener(ofEvents().mouseMoved   , this, &Layer_collage_manual::onMouseMoved);
    ofRemoveListener(ofEvents().mousePressed , this, &Layer_collage_manual::onMousePressed);
    ofRemoveListener(ofEvents().mouseScrolled, this, &Layer_collage_manual::onMouseScrolled);

}

//--------------------------------------------------------------
void Layer_collage_manual::onModeEditing()
{
    ofAddListener(ofEvents().mouseMoved   , this, &Layer_collage_manual::onMouseMoved);
    ofAddListener(ofEvents().mousePressed , this, &Layer_collage_manual::onMousePressed);
    ofAddListener(ofEvents().mouseScrolled, this, &Layer_collage_manual::onMouseScrolled);
}
