#pragma once
#include "ofMain.h"
#include "Utils/AutoShader.h"
#include "Utils/shader_base.h"
#include "Layer_Manager.h"

class Layer_factory;

#define REDRAW true
#define NO_REDRAW false
#define MAX_MASK_BRUSH_SIZE 100.0
#define MIN_MASK_BRUSH_SIZE 10.0

class Layer_base
{
public:
    Layer_base(string _name, int _instance, Layer_Manager * _layer_manager);
    virtual ~Layer_base();

    
    Layer_base(const Layer_base&) = delete;
    Layer_base& operator=(const Layer_base&) = delete;

    void setup(int _height, int width);

    virtual bool draw(pingPongFbo & mainFbo, bool _forceRedraw = false) const = 0;
    void drawGui();
    void drawOverlay(ofFbo & overlayFbo);

    void saveLayer();
    void saveMask();

    static void registerType(const string& name, Layer_factory *factory);
    static Layer_base *create(const string &name, Layer_Manager * _layer_manager);

    static bool isFilter(const string &name);

    static vector<string> get_layer_names();

    void update() { onUpdate(); };
    void reset() { clearFbo(); onReset(); redraw(); };

    void activate() {
        if (!b_active) {
            b_active = true;
            onSetupListeners();
            onActivate();
        }
    };
    void deactivate() {
        if (b_active) {
            p_editMask = false;
            b_active   = false;
            clearListeners();
            onDeactivate();
        }
    };

    void destroy(){
        deactivate();
        onDestroy();
    }


    void redraw() const { setRedraw(true); }
    void setRedraw(bool _redraw)   const { b_redraw = _redraw; }
    bool needsRedraw() const { return b_redraw; }

    void resize( int width, int height);

    const string& get_name() const { return name + ofToString(instance); }
    const string get_display_name() const { 
        if(customName == "" ) return name; 
        else                  return customName;
    }
    void set_display_name(const string & newName) {  customName = newName; }

    ofParameterGroup params;

protected:
    void clearFbo() const;  
    void clearListeners();  
    void clearMaskFbo() const;  

    virtual void onSetup()         {};
    virtual void onSetupParams()   {};
    virtual void onSetupListeners(){};
    virtual void onDrawGui()       {};
    virtual void onDrawOverlay()   {};
    virtual void onUpdate()        {};
    virtual void onReset()         {};    
    virtual void onActivate()      {};
    virtual void onDeactivate()    {};
    virtual void onDestroy()       {};
    virtual void onResize()        {};

    virtual void onMask()    const;

    virtual void handle_mask(const string & _path);

    string name, customName;
    int instance;
    Layer_Manager * layer_manager;
    bool b_active;

    ofParameter<bool> p_reset;
    ofParameter<bool> p_disable;
    ofParameter<bool> p_debugRedraw;
    ofParameter<bool> p_loadMask;
    ofParameter<bool> p_pause;

    ofParameter<bool> p_clearMask;
    ofParameter<bool> p_showMask;
    ofParameter<bool> p_mask;
    ofParameter<bool> p_invertMask;
    ofParameter<bool> p_editMask;


    ofEventListener l_paramsChanged;
    ofEventListener l_debugRedraw;

    ofEventListener l_onMouseMoved;
    ofEventListener l_onMouseDragged;
    ofEventListener l_onMousePressed;
    ofEventListener l_onMouseReleased;
    ofEventListener l_onMouseScrolled;
    ofEventListener l_onMouseEntered;
    ofEventListener l_onMouseExited;
    ofEventListener l_onFileDragged;

    glm::vec2 size;
    mutable pingPongFbo fbo;
    mutable ofFbo maskFbo;

private:
    void setupFbo(int w, int h); 
    void onResetInternal(bool & b_reset);

    void onLoadMask (bool & _val);
    void onClearMask(bool & _val);
    void onEditMask (bool & _val);

    void onMaskEditMousePressed ( ofMouseEventArgs & _args);
    void onMaskEditMouseDragged ( ofMouseEventArgs & _args);
    void onMaskEditMouseScrolled( ofMouseEventArgs & _args);
    void drawMaskEditBrush      ( ofMouseEventArgs & _args);
    void drawMaskBrush() const;
    void drawMasked() const;

    static map<string, Layer_factory*>* GetFactoryDirectory() {        
        static map<string, Layer_factory*>* factories = new map<string, Layer_factory*>();
        return factories;
    }

    mutable bool b_redraw;

    shared_ptr<AutoShader> mask_shader;
    ofEventListener        l_onShaderLoad;

    glm::vec2 maskBrushPosition;
    float     maskBrushSize = 50;

};

class Static_base : public Layer_base
{
public:
    Static_base(string name, int instance, Layer_Manager * layer_manager )  : Layer_base(name, instance, layer_manager) {};
    bool draw(pingPongFbo & mainFbo, bool _forceRedraw = false) const override;

protected:
    virtual void onDraw()   const {};
    virtual void onRender() const {};
};

class Filter_base  : public Layer_base
{
public:
    Filter_base(string name, int instance, Layer_Manager * layer_manager) : Layer_base(name, instance, layer_manager) { redraw(); };
    bool draw(pingPongFbo & mainFbo, bool _forceRedraw = false) const override;
protected:
    virtual void onDraw(const ofTexture & _baseTex) const {};
    virtual void onRender(const ofTexture & _baseTex) const {};

};

class Layer_factory
{
public:
    virtual Layer_base *create(Layer_Manager * _layer_manager) = 0;
    virtual bool isFilter() = 0;
protected:
    string class_name;
    string display_name;
    int    instance;
};

#define REGISTER_TYPE(klass, name) \
    class klass##_factory : public Layer_factory { \
    public: \
        klass##_factory() \
        { \
            Layer_base::registerType(#name, this); \
            class_name   = #klass; \
            display_name = #name; \
        } \
        virtual Layer_base *create(Layer_Manager * _layer_manager) { \
            return new klass( display_name, instance++ , _layer_manager); \
        } \
        virtual bool isFilter() { \
            return std::is_base_of<Filter_base, klass>::value; \
        } \
    }; \
    static klass##_factory global_##klass##_factory;

