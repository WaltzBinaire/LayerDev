#pragma once
#include "Layers\layer_file.h"
#include "ofxCollageViewer.h"

class Layer_file_aiCollage : public Layer_file
{
public:
    enum class MODE { NONE, EDITING, PLACING};

    Layer_file_aiCollage(string name, int instance, Layer_Manager * _layer_manager) : Layer_file(name, instance, _layer_manager) {};

    virtual const string get_display_name() const override { return "Collage"; }

protected:
    virtual void onSetupParams() override ;
    virtual void onActivate() override;
    virtual void onDeactivate() override;
    virtual void onRender() const override;    
    virtual void onDraw()  const override;    
    virtual void onUpdate() override;
    virtual void onReset()  override;

    void onModeChanged(int & _mode);
    void addDefaultListeners();
    void addCollageListeners();        
    void removeDefaultListeners();
    void removeCollageListeners();

    virtual float getFileWidth()  const { return m_collage.getWidth(); };
    virtual float getFileHeight() const { return m_collage.getHeight(); };

    ofParameter<int> p_mode;

    virtual vector<string> & get_allowed_exts() { return allowed_exts; }
    virtual void handle_file(const string & _path);

private:
    static vector<string> allowed_exts;

    MODE mode;

    Collage m_collage;
};
