#pragma once
#include "ofMain.h"
#include "Utils\pingPongFbo.h"
#include "ProjectManager.h"

namespace LayerUtils {

    template <class Caller>
    bool loadFileDialogue(const vector<string> & allowed_exts, Caller * caller, void (Caller::*_handle_file)(const string&))
    {
        ProjectManager & projectManager = ProjectManager::getInstance();
        
        string defaultPath = "";
        if (projectManager.isLoaded()) defaultPath = projectManager.getPath();

        auto result = ofSystemLoadDialog("Load File", false, defaultPath );

        if (result.bSuccess) {
            ofFile file(result.getPath());

            if (!file.exists()) {
                ofLogWarning(__FUNCTION__) << "Cannot load: " << file.path();
                return false;
            }

            if (std::find(allowed_exts.begin(), allowed_exts.end(), file.getExtension()) == allowed_exts.end()) {
                ofLogWarning(__FUNCTION__) << "Cannot load: " << file.path() << "\nWrong type";
                return false;
            }
            ofLogVerbose(__FUNCTION__) << "Handling " << file.path();
            (*caller.*_handle_file)(file.path());
        }
        return true;
    }
    
    template <class Caller>
    bool loadFolderDialogue(Caller * caller, void (Caller::*_handle_folder)(const string&))
    {

        ProjectManager & projectManager = ProjectManager::getInstance();
        
        string defaultPath = "";
        if (projectManager.isLoaded()) defaultPath = projectManager.getPath();

        auto result = ofSystemLoadDialog("Open Folder...", true, defaultPath );

        if (result.bSuccess) {
            ofDirectory folder(result.getPath());

            if (!folder.exists()) {
                ofLogWarning(__FUNCTION__) << "Cannot open: " << folder.path();
                return false;
            }

            ofLogVerbose(__FUNCTION__) << "Handlinga " << folder.path();
            (*caller.*_handle_folder)(folder.path());
        }
        return true;
    }

    bool saveFolderDialogue(string & path);

    static const vector<string> img_exts  {
            "jpg",
            "JPG",
            "jpeg",
            "JPEG",
            "png",
            "PNG",
            "gif",
            "GIF"
    };

    static const vector<string> collage_exts  { "svg", "SVG" };


    bool saveImageDialogue(string & path);

    bool saveImage(const string path, const ofImage      & image  );
    bool saveImage(const string path, const ofTexture    & texture);
    bool saveImage(const string path, const ofPixels     & pixels );
    bool saveImage(const string path, const ofFbo        & fbo    );
    bool saveImage(const string path, const pingPongFbo  & fbo    );


    class UVQuad {
    public:
        static UVQuad & getInstance() {
            static UVQuad quad;            
            return quad;
        }

        void draw(glm::vec2 pos, glm::vec2 size) { draw(pos.x, pos.y, size.x, size.y);  }

        void draw(float x, float y, float w, float h) {
            mesh.setVertex(0, glm::vec3( x    , y    , 0 ));
	        mesh.setVertex(1, glm::vec3( x + w, y    , 0 ));
	        mesh.setVertex(2, glm::vec3( x + w, y + h, 0 ));
	        mesh.setVertex(3, glm::vec3( x    , y + h, 0 ));

            mesh.draw();
        }

    private:
        UVQuad() {
            mesh.addVertex(glm::vec3(0));
        	mesh.addVertex(glm::vec3(0));
        	mesh.addVertex(glm::vec3(0));
        	mesh.addVertex(glm::vec3(0));
        			
        	mesh.addTexCoord(glm::vec2(0.0, 0.0));
        	mesh.addTexCoord(glm::vec2(1.0, 0.0));
        	mesh.addTexCoord(glm::vec2(1.0, 1.0));
        	mesh.addTexCoord(glm::vec2(0.0, 1.0));
        
            mesh.addIndex(0);
            mesh.addIndex(1);
            mesh.addIndex(2);
        
            mesh.addIndex(2);
            mesh.addIndex(3);
            mesh.addIndex(0);
        }

        ofMesh mesh;


    };
}