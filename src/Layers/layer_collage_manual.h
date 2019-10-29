#pragma once
#include "ofMain.h"
#include "Layers\layer_collage.h"

class Layer_collage_manual : public Layer_collage
{
public:
    Layer_collage_manual(string name, int instance, Layer_Manager * _layer_manager)
        : Layer_collage(name, instance, _layer_manager) {}

protected:

    virtual void onSetupListeners()  override ;
    
    virtual void setupPatch(CollagePatch & _patch, int _idx) override;

    void onFileDragEvent(ofDragInfo & _fileInfo);

    void onMouseMoved(ofMouseEventArgs & _args);
    void onMousePressed(ofMouseEventArgs & _args);
    void onMouseScrolled(ofMouseEventArgs & _args);

    virtual void onModeViewing() override;
    virtual void onModeEditing() override;
};

