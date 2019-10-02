#pragma once
#include "ofMain.h"
#include "AutoShader.h"
#include "shader_base.h"
#include "Layer_Manager.h"

class Layer_factory;

class Layer_base
{
public:
    Layer_base(string _name, Layer_Manager * _layer_manager);
    virtual ~Layer_base();

    void setup(int _height, int width);
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

    const string& get_name() const { return name; }
    virtual const string get_display_name() const { return get_name(); }

    ofParameterGroup params;

protected:
    void clearFbo() const;;    
    void drawTexture(const ofTexture & _baseTex)const;

    virtual void onSetup()       {};
    virtual void onSetupParams() {};
    virtual void onUpdate()      {};
    virtual void onReset()       {};    
    virtual void onActivate()    {};
    virtual void onDeactivate()  {};
    virtual void onDestroy()     {};
    virtual void onResize()      {};

    string name;
    Layer_Manager * layer_manager;
    bool b_active;

    ofParameter<bool> p_reset;
    ofParameter<bool> p_disable;

    ofEventListener l_paramsChanged;

    glm::vec2 size;
    mutable ofFbo fbo;

private:
    mutable bool b_redraw;

    static map<string, Layer_factory*>* GetFactoryDirectory() {        
        static map<string, Layer_factory*>* factories = new map<string, Layer_factory*>();
        return factories;
    }
    void onResetInternal(bool & b_reset);
    void setupFbo(int w, int h);  
    void quadSetup();
    void setQuad(const ofTexture & _baseTex) const;
    
    
    shared_ptr<AutoShader> base_shader;
    mutable ofMesh baseQuad;
};

class Static_base : public Layer_base
{
public:
    Static_base(string name, Layer_Manager * layer_manager )  : Layer_base(name, layer_manager) {};
    bool draw() const;
protected:
    virtual void onDraw() const {};
};

class Filter_base  : public Layer_base
{
public:
    Filter_base(string name, Layer_Manager * layer_manager) : Layer_base(name, layer_manager) { redraw(); };
    bool draw(const ofTexture & _baseTex, bool _forceRedraw = false) const;

protected:
    virtual void onDraw(const ofTexture & _baseTex) const {};

};

class Layer_factory
{
public:
    virtual Layer_base *create(Layer_Manager * _layer_manager) = 0;
protected:
    string name;
    int    number;
};

#define REGISTER_TYPE(klass) \
    class klass##_factory : public Layer_factory { \
    public: \
        klass##_factory() \
        { \
            Layer_base::registerType(#klass, this); \
            name = #klass; \
        } \
        virtual Layer_base *create(Layer_Manager * _layer_manager) { \
            return new klass( name + "_" + ofToString(number++ ), _layer_manager); \
        } \
    }; \
    static klass##_factory global_##klass##_factory;

