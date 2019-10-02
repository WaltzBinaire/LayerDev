#pragma once
#include "layer_filter_alpha_replace.h"

class Layer_alpha_replace_load : public Layer_filter_alpha_replace
{
public:
    Layer_alpha_replace_load(string name, Layer_Manager * _layer_manager)
        : Layer_filter_alpha_replace(name, _layer_manager) {};

protected:

    virtual void onSetupParams() override;
    virtual void onActivate()    override ;
    virtual void onDeactivate()  override ;

    virtual ofTexture & getAlphaTexture() const override { return ofTexture(); };

};

