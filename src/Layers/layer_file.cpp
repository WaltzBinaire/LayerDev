#include "Layers\layer_file.h"
#include "Utils\LayerUtils.h"
#include "GUI/SingleLayerGui.h"

void Layer_file::onDrawGui()
{
    SingleLayerGui::specialisedDrawGui<Layer_file>(this); 
}

void Layer_file::onSetupParams()
{
    p_loadFolder.set("Load", false);
    p_loadFolder.addListener(this, &Layer_file::onLoadFile);

    params.add(
        p_loadFolder
    );
}

void Layer_file::onSetupListeners()
{    
    l_onFileDragged   = ofEvents().fileDragEvent.newListener(           this, &Layer_file::onFileDragEvent);
    l_onMousePressed  = layer_manager->canvasMousePressed.newListener ( this, &Layer_file::onMousePressed);
    l_onMouseScrolled = layer_manager->canvasMouseScrolled.newListener( this, &Layer_file::onMouseScrolled);
    l_onMouseMoved    = layer_manager->canvasMouseMoved.newListener   ( this, &Layer_file::onMouseMoved);
}   

void Layer_file::onFileDragEvent(ofDragInfo & _fileInfo)
{
    if (_fileInfo.files.size() > 1) ofLogWarning() << "Can't add multiple images.";

    string file_path = _fileInfo.files[0];
    ofFile file(file_path);

    if (!file.exists()) {
        ofLogWarning(name) << "Cannot load: " << file.path();
        return;
    }

    const vector<string> & allowed_exts = get_allowed_exts();

    if (std::find(allowed_exts.begin(), allowed_exts.end(), file.getExtension()) == allowed_exts.end()) {
        ofLogWarning(name) << "Cannot load: " << file.path() << "\nWrong type";
        return;
    }
    ofLogVerbose(name) << "Handling " << file.path();
    handle_file(file.path());
}

void Layer_file::onLoadFile(bool & _loadFile)
{
    if (_loadFile) {
        LayerUtils::loadFileDialogue(get_allowed_exts(), this, &Layer_file::handle_file);
    }
    _loadFile = false;
}

void Layer_file::onMousePressed(ofMouseEventArgs & args)
{
    b_placing = !b_placing;
    clickPosition = glm::vec2(args.x, args.y);
    initPosition = position;
}

void Layer_file::onMouseScrolled(ofMouseEventArgs & args)
{
    if (b_placing) {
        scale += args.scrollY * 0.05;
        scale = max(0.1f, scale);
        redraw();
    } 
}

void Layer_file::onMouseMoved(ofMouseEventArgs & args)
{
    if (b_placing) {
        position = initPosition + ( glm::vec2(args.x, args.y) - clickPosition);
        redraw();
    }
}
