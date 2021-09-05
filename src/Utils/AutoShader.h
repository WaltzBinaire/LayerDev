#pragma once
#include "ofMain.h"
#include <functional>

struct UniformFunction {
    UniformFunction() {
    }

    UniformFunction(std::function<void(ofShader*)> _assign) {
        assign = _assign;
    }

    std::function<void(ofShader*)>  assign;
    bool                            present = true;
};

typedef map<string, UniformFunction> UniformFunctionsList;

class AutoShader : public ofShader {
public:
    AutoShader();
    virtual ~AutoShader();
    
    // override the initialisation functions
    bool            load(const string &_shaderName);
    bool            load(const string &_vertName, const string &_fragName);
    
    bool            reloadShaders();
        
    void            enableWatchFiles();
    void            disableWatchFiles();

    void            setMillisBetweenFileCheck(int _millis);
    
    void            addIncludeFolder(const string &_folder);
    
    void            addDefineKeyword(const string &_define);
    void            delDefineKeyword(const string &_define);

    string          getFilename(GLenum _type);
    
    void            begin();

    ofEvent<bool>   onLoad;
    ofEvent<bool>   onChange;
    
protected:
    void                    _update(ofEventArgs &e);

    vector<string>          m_defines;
    vector<string>          m_includeFolders;
    
    UniformFunctionsList    m_uniformsFunctions;

    double                  m_lastFrame;

private:
    std::time_t     _getLastModified(ofFile& _file);
    bool            _filesChanged();
    
    vector<time_t>  m_fileChangedTimes;
    
    ofFile          m_vertexShaderFile;
    ofFile          m_fragmentShaderFile;
    
    string          m_vertexShaderFilename;
    string          m_fragmentShaderFilename;
    
    GLenum          m_geometryInputType;
    GLenum          m_geometryOutputType;
    
    int             m_lastTimeCheckMillis;
    int             m_millisBetweenFileCheck;
    int             m_version;
    
    bool            m_bWatchingFiles;
    bool            m_loadShaderNextFrame;
};

