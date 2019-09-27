#include "AutoShader.h"

AutoShader::AutoShader() {
    m_bWatchingFiles = false;

    // TIME UNIFORMS
    //
    m_uniformsFunctions["u_time"] = UniformFunction( [](ofShader* _shader) {
        _shader->setUniform1f("u_time", ofGetElapsedTimef());
    });

    m_uniformsFunctions["u_delta"] = UniformFunction( [this](ofShader* _shader) {
        double now = ofGetElapsedTimef();
        _shader->setUniform1f("u_delta", now - m_lastFrame);
        m_lastFrame = now;
    });

    m_uniformsFunctions["u_date"] = UniformFunction( [](ofShader* _shader) {
        _shader->setUniform4f("u_date", ofGetYear(), ofGetMonth(), ofGetDay(), ofGetSeconds());
    });

    // MOUSE
    m_uniformsFunctions["u_mouse"] = UniformFunction( [](ofShader* _shader) {
        _shader->setUniform2f("u_mouse", ofGetMouseX(), ofGetMouseY());
    } );

    // VIEWPORT
    m_uniformsFunctions["u_resolution"]= UniformFunction( [](ofShader* _shader) {
        _shader->setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    });
}

AutoShader::~AutoShader() {
    disableWatchFiles();
}

void AutoShader::addIncludeFolder(const string &_folder) {
    m_includeFolders.push_back(_folder);
    m_loadShaderNextFrame = true;
}

void AutoShader::addDefineKeyword(const string &_define) {
    m_defines.push_back(_define);
    m_loadShaderNextFrame = true;
}

void AutoShader::delDefineKeyword(const string &_define) {
    for (int i = m_defines.size() - 1; i >= 0 ; i++) {
        if ( m_defines[i] == _define ) {
            m_defines.erase(m_defines.begin() + i);
        }
    }
}

string getAbsPath(const string& _str) {
    // string abs_path = realpath(_str.c_str(), NULL);
      string abs_path = ofFilePath::getAbsolutePath(_str);
    std::size_t found = abs_path.find_last_of("\\/");
    if (found) {
        return abs_path.substr(0, found);
    }
    else {
        return "";
    }
}

string urlResolve(const string& _path, const string& _pwd, const std::vector<string> _includeFolders) {
    string url = _pwd + '/' + _path;
    if (ofFile(url).exists()) {
        return url;
    }
    else {
        for (unsigned int i = 0; i < _includeFolders.size(); i++) {
            string new_path = _includeFolders[i] + "/" + _path;
            if (ofFile(new_path).exists()) {
                return new_path;
            }
        }
        return _path;
    }
}

bool loadFromPath(const string& _path, string* _into, const std::vector<string> _includeFolders) {
    std::ifstream file;
    string buffer;
    
    file.open(_path.c_str());
    if (!file.is_open()) 
        return false;
    string original_path = getAbsPath(_path);
    
    while (!file.eof()) {
        getline(file, buffer);
        if (buffer.find("#include ") == 0 || buffer.find("#pragma include ") == 0){
            unsigned begin = buffer.find_first_of("\"");
            unsigned end = buffer.find_last_of("\"");
            if (begin != end) {
                string file_name = buffer.substr(begin+1,end-begin-1);
                file_name = urlResolve(file_name, original_path, _includeFolders);
                string newBuffer;
                if (loadFromPath(file_name, &newBuffer, _includeFolders)) {
                    (*_into) += "\n" + newBuffer + "\n";
                }
                else {
                    std::cout << file_name << " not found at " << original_path << std::endl;
                }
            }
        } 
        else {
            (*_into) += buffer + "\n";
        }
    }
    
    file.close();
    return true;
}

bool _find_id(const string& program, const char* id) {
    return std::strstr(program.c_str(), id) != 0;
}

bool AutoShader::load(const string &_shaderName ) {
    return load( _shaderName + ".vert", _shaderName + ".frag" );
}

bool AutoShader::load(const string &_vertName, const string &_fragName) {
    unload();
        
    ofShader::setGeometryInputType( m_geometryInputType );
    ofShader::setGeometryOutputType( m_geometryOutputType );

    // hackety hack, clear errors or shader will fail to compile
    // GLuint err = glGetError();

    m_lastTimeCheckMillis = ofGetElapsedTimeMillis();
    setMillisBetweenFileCheck( 2 * 1000 );
    enableWatchFiles();

    m_loadShaderNextFrame = false;

    // Update filenames
    m_vertexShaderFilename = _vertName;
    m_fragmentShaderFilename = _fragName;

    // Update last change time
    m_vertexShaderFile.clear();
    m_fragmentShaderFile.clear();

    m_vertexShaderFile   = ofFile( ofToDataPath( m_vertexShaderFilename ) );
    m_fragmentShaderFile = ofFile( ofToDataPath( m_fragmentShaderFilename ) );
    
    m_fileChangedTimes.clear();
    m_fileChangedTimes.push_back( _getLastModified( m_vertexShaderFile ) );
    m_fileChangedTimes.push_back( _getLastModified( m_fragmentShaderFile ) );
    
    // Update Sources
    string vertexSrc = "";
    string fragmentSrc = "";
    
    // 1. Load shaders resolving #include to nested sources
    loadFromPath( ofToDataPath( m_vertexShaderFilename ), &vertexSrc, m_includeFolders );
    loadFromPath( ofToDataPath( m_fragmentShaderFilename ), &fragmentSrc, m_includeFolders );

    if (vertexSrc.size() == 0) {
        vertexSrc = "\n\
uniform mat4    modelViewProjectionMatrix;\n\
\n\
attribute vec4  position;\n\
attribute vec4  color;\n\
attribute vec2  texcoord;\n\
\n\
varying vec4    v_position;\n\
varying vec4    v_color;\n\
varying vec2    v_texcoord;\n\
\n\
void main() {\n\
    v_position  = position;\n\
    v_color = color;\n\
    v_texcoord  = texcoord;\n\
    gl_Position = modelViewProjectionMatrix * v_position;\n\
}\n";
    }
    
    // 2. Add defines
    string defines_header = "";
    for (unsigned int i = 0; i < m_defines.size(); i++) {
        defines_header += "#define " + m_defines[i] + "\n";
    }
    defines_header += "#line 0 \n";
    
    // 2. Check active default uniforms
    for (UniformFunctionsList::iterator it = m_uniformsFunctions.begin(); it != m_uniformsFunctions.end(); ++it) {
        it->second.present = (  _find_id(vertexSrc, it->first.c_str()) != 0 || 
                                _find_id(fragmentSrc, it->first.c_str()) != 0 );
    }
    
    // 3. Add defines
    string version_vert_header = vertexSrc.substr(0, vertexSrc.find("\n", 0)) + "\n";
    string version_frag_header = fragmentSrc.substr(0, fragmentSrc.find("\n", 0)) + "\n";

    vertexSrc = vertexSrc.substr(vertexSrc.find("\n", 0), vertexSrc.size());
    fragmentSrc = fragmentSrc.substr(fragmentSrc.find("\n", 0), fragmentSrc.size());
            
    if ( vertexSrc.size() > 0 ) {
        setupShaderFromSource( GL_VERTEX_SHADER, version_vert_header + defines_header + vertexSrc );
    }

    if ( fragmentSrc.size() > 0 ) {
        setupShaderFromSource( GL_FRAGMENT_SHADER, version_frag_header + defines_header + fragmentSrc );
    }
   

    bindDefaults();
        
    bool link = linkProgram();
    ofNotifyEvent(onLoad, link);//, this);
    return link;;
}

std::string AutoShader::getFilename(GLenum _type) {
    switch (_type) {
        case GL_FRAGMENT_SHADER:
            return m_fragmentShaderFilename;
            break;
        case GL_VERTEX_SHADER:
            return m_vertexShaderFilename;
            break;
        default:
             return "";
             break;
    }
}

void AutoShader::begin() {
    ofShader::begin();

    for (UniformFunctionsList::iterator it = m_uniformsFunctions.begin(); it != m_uniformsFunctions.end(); ++it) {
        if (it->second.present) {
            if (it->second.assign) {
                it->second.assign((ofShader*)this);
            }
        }
    }
}

void AutoShader::_update (ofEventArgs &e) {
    if ( m_loadShaderNextFrame ) {
        reloadShaders();
        m_loadShaderNextFrame = false;
    }
        
    int currTime = ofGetElapsedTimeMillis();
        
    if (((currTime - m_lastTimeCheckMillis) > m_millisBetweenFileCheck) &&
        !m_loadShaderNextFrame ) {
        if ( _filesChanged() ) {
            m_loadShaderNextFrame = true;
            ofNotifyEvent(onChange, m_loadShaderNextFrame, this);
        }
        
        m_lastTimeCheckMillis = currTime;
    }
}

bool AutoShader::reloadShaders() {
    return load( m_vertexShaderFilename,  m_fragmentShaderFilename );
}

void AutoShader::enableWatchFiles() {
    if (!m_bWatchingFiles) {
        ofAddListener( ofEvents().update, this, &AutoShader::_update );
        m_bWatchingFiles = true;
    }
}

void AutoShader::disableWatchFiles() {
    if (m_bWatchingFiles) {
        ofRemoveListener( ofEvents().update, this, &AutoShader::_update );
        m_bWatchingFiles = false;
    }
}

bool AutoShader::_filesChanged() {
    bool fileChanged = false;
        
    if ( m_vertexShaderFile.exists() ) {
        std::time_t vertexShaderFileLastChangeTime = _getLastModified( m_vertexShaderFile );
        if ( vertexShaderFileLastChangeTime != m_fileChangedTimes.at(0) ) {
            m_fileChangedTimes.at(0) = vertexShaderFileLastChangeTime;
            fileChanged = true;
        }
    }
        
    if ( m_fragmentShaderFile.exists() ) {
        std::time_t fragmentShaderFileLastChangeTime = _getLastModified( m_fragmentShaderFile );
        if ( fragmentShaderFileLastChangeTime != m_fileChangedTimes.at(1) ) {
            m_fileChangedTimes.at(1) = fragmentShaderFileLastChangeTime;
            fileChanged = true;
        }
    }
    return fileChanged;
}

std::time_t AutoShader::_getLastModified( ofFile& _file ) {
    if ( _file.exists() ) {
        return std::filesystem::last_write_time(_file.path());
    }
    else {
        return 0;
    }
}

void AutoShader::setMillisBetweenFileCheck( int _millis ) {
    m_millisBetweenFileCheck = _millis;
}

