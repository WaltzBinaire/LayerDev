#pragma once
#include "Layers\layer_file.h"
#include "ofxCollageViewer.h"

class Layer_file_aiCollage : public Layer_file
{
public:
    enum class MODE { NONE, EDITING, PLACING};

    static const vector<string> & getModeNames() {
        static const vector<string> modeNames{ "View", "Edit", "Place" };
        return modeNames;
    }

    Layer_file_aiCollage(string name, int instance, Layer_Manager * _layer_manager) : Layer_file(name, instance, _layer_manager) {};

    virtual void handle_file(const string & _path);

    void Sort()            { m_collage.sortByLoss();       }
    void SortInternal()    { m_collage.sortInternal();     }
    void Shuffle()         { m_collage.randomGroupOrder(); }
    void ShuffleInternal() { m_collage.randomInternal();   }

protected:
    virtual void onSetupParams()    override ;
    virtual void onSetupListeners() override ;
    virtual void onDeactivate()  override;
    virtual void onRender(bool _forced = false) const override;    
    virtual void onDraw  (bool _forced = false) const override;  
    virtual void onDrawOverlay() override;  
    virtual void onDrawGui()     override;
    virtual void onUpdate()      override;
    virtual void onReset()       override;

    void onModeChanged(int & _mode);
    void addCollageListeners();       

    void setListeners();

    void onMouseMovedCollage(ofMouseEventArgs & _args);

    virtual float getFileWidth()  const { return m_collage.getWidth(); };
    virtual float getFileHeight() const { return m_collage.getHeight(); };

    ofParameter<int> p_mode;

    virtual vector<string> & get_allowed_exts() { return allowed_exts; }


private:
    static vector<string> allowed_exts;

    MODE mode;

    Collage m_collage;
};
