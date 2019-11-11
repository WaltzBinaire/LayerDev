#pragma once
#include "ofMain.h"
#include "Utils/LayerUtils.h"
#include "Utils/threadedImageLoader.h"


#include "Utils/FaceExtractor.h"


#define THUMBNAIL_SIZE 150

class ProjectResource {

public:

    enum class RESOURCE_TYPE {  
        RAW, 
        SEGMENTED, 
        TARGET, 
        COLLAGE, 
        MASKS, 
        FACES,
        PRECOLLAGE
    };
    ProjectResource(RESOURCE_TYPE _rt) : rt(_rt) {};
    ~ProjectResource();

    //const vector<ofTexture> & getThumbnails() const;
    //int getNumFiles() const { return thumbnail_images.size(); };
    //int getNumLoadedFiles() const { return thumbnail_textures.size(); };

    string getFilePath(int i) const { 
        if (i < filePaths.size()) return filePaths[i];
        else return "";
    }

    size_t size() const { return filePaths.size(); }

    string getDirectoryPath() const { 
        return rootDir.path();
    }

    bool setup(const string & _path);

    vector<string> get_allowed_exts();

    static const map<RESOURCE_TYPE, string> resource_rel_paths;
    static const map<RESOURCE_TYPE, string> resource_names;
    static const char * resource_name_c[];

private:

    void scanDir(ofDirectory & dir, const vector<string>  & allowedExts);

    //void loadThumbnails();
    //void loadCollageThumbnails();

    ofDirectory rootDir;
    vector<string> filePaths;

    //vector<ofImage*>   thumbnail_images;
    //mutable vector<ofTexture> thumbnail_textures;

    //threadedImageLoader imageLoader;

    RESOURCE_TYPE rt;

};


class ProjectManager
{
        using RESOURCE_TYPE = ProjectResource::RESOURCE_TYPE ;
    public:

        static ProjectManager& getInstance();

        void loadProject();
        void clear();

        void populateFaceFolder();

        bool isLoaded() const { return b_isLoaded; };

        const shared_ptr<ProjectResource> getResource(RESOURCE_TYPE _rt) const;

        string getName()       const { return name; };
        string getPath()       const { return root_path; };
        string getAssetsPath() const { return ofFilePath::join(root_path, "Assets"); };
        string getOutputPath() const { return ofFilePath::join(root_path, "Output"); };

        ProjectManager(ProjectManager const&)  = delete;
        void operator=(ProjectManager const&)  = delete;

        ofEvent<bool> onLoaded;

    private:

        ProjectManager():
            b_isLoaded(false)
        {};

        void loadProject(const string & _root_path);

        void createResource(ProjectResource::RESOURCE_TYPE rt, const string & resource_path);

        bool b_isLoaded;

        map<RESOURCE_TYPE, shared_ptr<ProjectResource>> resources;

        string root_path;
        string name;
        vector<ofTexture> thumbnails;

        ofEventListener l_onFaceExtractorComplete;

};