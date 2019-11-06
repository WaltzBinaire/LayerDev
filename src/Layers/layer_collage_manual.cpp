#include "Layers\layer_collage_manual.h"

REGISTER_TYPE(Layer_collage_manual, Manual Collage)

void Layer_collage_manual::onSetupListeners()
{
    l_onFileDragged = ofEvents().fileDragEvent.newListener( this, &Layer_collage_manual::onFileDragEvent);
}


void Layer_collage_manual::setupPatch(CollagePatch & _patch, int _idx)
{
        _patch.setup(size * 0.5, 1.0, 0.0);
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

    ofImage newImage = ofImage(file.getAbsolutePath());

    if (newImage.isAllocated()) {
        images.emplace_back( new CollagePatch() );
        active_image = images.back();        
        setupPatch(*active_image, 0);
        active_image->getImageReference() = newImage;


        setMode(Mode::EDITING);

        ofLogVerbose() << "Added image: " << file.path();
    }
}
//--------------------------------------------------------------
void Layer_collage_manual::onMouseMoved(ofMouseEventArgs & _args)
{
    if (mode == Mode::EDITING) {
        active_image->setCenter( glm::vec2(_args.x, _args.y));
    }
}

//--------------------------------------------------------------
void Layer_collage_manual::onMousePressed(ofMouseEventArgs & _args)
{
    setMode(Mode::VIEWING);
}

//--------------------------------------------------------------
void Layer_collage_manual::onMouseScrolled(ofMouseEventArgs & _args)
{
    active_image->setScale( active_image->getScale() + _args.scrollY / 20.0f);
}

//--------------------------------------------------------------
void Layer_collage_manual::onModeViewing()
{
    ofRemoveListener(layer_manager->canvasMouseMoved   , this, &Layer_collage_manual::onMouseMoved);
    ofRemoveListener(layer_manager->canvasMousePressed , this, &Layer_collage_manual::onMousePressed);
    ofRemoveListener(layer_manager->canvasMouseScrolled, this, &Layer_collage_manual::onMouseScrolled);

}

//--------------------------------------------------------------
void Layer_collage_manual::onModeEditing()
{
    ofAddListener(layer_manager->canvasMouseMoved   , this, &Layer_collage_manual::onMouseMoved);
    ofAddListener(layer_manager->canvasMousePressed , this, &Layer_collage_manual::onMousePressed);
    ofAddListener(layer_manager->canvasMouseScrolled, this, &Layer_collage_manual::onMouseScrolled);
}
