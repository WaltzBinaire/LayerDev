#pragma once
#include "ofMain.h"
#include "Utils/AutoShader.h"
#include "Utils/shader_base.h"
#include "Layer_Manager.h"

class Layer_factory;

#define REDRAW true
#define NO_REDRAW false

class Layer_base
{
public:
    Layer_base(string _name, int _instance, Layer_Manager * _layer_manager);
    virtual ~Layer_base();

    void setup(int _height, int width);

    virtual bool draw(pingPongFbo & mainFbo, bool _forceRedraw = false) const = 0;
    void drawGui();

    static void registerType(const string& name, Layer_factory *factory);
    static Layer_base *create(const string &name, Layer_Manager * _layer_manager);
    static vector<string> get_layer_names();

    void update() { onUpdate(); };
    void reset() { clearFbo(); onReset(); redraw(); };

    void activate() {
        if (!b_active) {
            b_active = true;
            onActivate();
        }
    };
    void deactivate() {
        if (b_active) {
            b_active = false;
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
    const string get_display_name() const { return name; }

    ofParameterGroup params;

protected:
    void clearFbo() const;  

    virtual void onSetup()         {};
    virtual void onSetupParams()   {};
    virtual void onDrawGui()       {};
    virtual void onUpdate()        {};
    virtual void onReset()         {};    
    virtual void onActivate()      {};
    virtual void onDeactivate()    {};
    virtual void onDestroy()       {};
    virtual void onResize()        {};

    virtual void onMask()    const;

    string name;
    int instance;
    Layer_Manager * layer_manager;
    bool b_active;

    ofParameter<bool> p_reset;
    ofParameter<bool> p_disable;
    ofParameter<bool> p_debugRedraw;
    ofParameter<bool> p_loadMask;
    ofParameter<bool> p_mask;
    ofParameter<bool> p_invertMask;
    ofParameter<bool> p_pause;


    ofEventListener l_paramsChanged;
    ofEventListener l_debugRedraw;

    glm::vec2 size;
    mutable pingPongFbo fbo;

    mutable ofTexture mask;
    glm::vec2 maskOffset;
    glm::vec2 maskScale;

private:
    void onLoadMask(bool & _val);
    void handle_mask(const string & _path);

    mutable bool b_redraw;

    static map<string, Layer_factory*>* GetFactoryDirectory() {        
        static map<string, Layer_factory*>* factories = new map<string, Layer_factory*>();
        return factories;
    }
    void onResetInternal(bool & b_reset);
    void setupFbo(int w, int h);  

    
    void drawMasked() const;

    void quadSetup();
    void setQuad(const ofTexture & _baseTex) const;
    
    
    shared_ptr<AutoShader> mask_shader;
    mutable ofMesh baseQuad;
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
    }; \
    static klass##_factory global_##klass##_factory;

