#pragma once
#include "ofMain.h"
#include "Utils/LayerUtils.h"
#include "Utils/threadedImageLoader.h"


#define THUMBNAIL_SIZE 150

class ProjectResource {

public:

    enum class RESOURCE_TYPE {  RAW, SEGMENTED, TARGET };
    ProjectResource(RESOURCE_TYPE _rt) : rt(_rt) {};
    ~ProjectResource();

    const vector<ofTexture> & getThumbnails() const;
    int getNumFiles() const { return thumbnail_images.size(); };
    int getNumLoadedFiles() const { return thumbnail_textures.size(); };

    bool setup(const string & _path);

    vector<string> get_allowed_exts();

    static const map<RESOURCE_TYPE, string> resource_rel_paths;
    static const map<RESOURCE_TYPE, string> resource_names;
    static const char * resource_name_c[];

private:

    void loadThumbnails();

    ofDirectory dir;
    vector<ofImage*>   thumbnail_images;
    mutable vector<ofTexture> thumbnail_textures;

    threadedImageLoader imageLoader;

    RESOURCE_TYPE rt;

};


class ProjectManager
{
        using RESOURCE_TYPE = ProjectResource::RESOURCE_TYPE ;
    public:

        static ProjectManager& getInstance();

        void loadProject();

        bool isLoaded() const { return b_isLoaded; };

        const shared_ptr<ProjectResource> getResource(RESOURCE_TYPE _rt) const;

        string getName() const { return name; };
        string getPath() const { return root_path; };

        ProjectManager(ProjectManager const&)  = delete;
        void operator=(ProjectManager const&)  = delete;

    private:


        ProjectManager():
            b_isLoaded(false)
        {};

        void loadProject(const string & _root_path);

        

        bool b_isLoaded;

        map<RESOURCE_TYPE, shared_ptr<ProjectResource>> resources;

        string root_path;
        string name;
        vector<ofTexture> thumbnails;

};