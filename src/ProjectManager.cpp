#include "ProjectManager.h"

const map<ProjectResource::RESOURCE_TYPE, string> ProjectResource::resource_rel_paths
{
    {RESOURCE_TYPE::RAW      , "Assets/Raw"       },
    {RESOURCE_TYPE::SEGMENTED, "Assets/Segmented" },
    {RESOURCE_TYPE::TARGET   , "Assets/Targets"   },
    {RESOURCE_TYPE::COLLAGE  , "Assets/Collage"   }
};

const char* ProjectResource::resource_name_c[] = {
    "Instagram",
    "Segmented",
    "Portrait",
    "Collage"};

const map<ProjectResource::RESOURCE_TYPE, string> ProjectResource::resource_names
{
    {RESOURCE_TYPE::RAW      , resource_name_c[0] },
    {RESOURCE_TYPE::SEGMENTED, resource_name_c[1] },
    {RESOURCE_TYPE::TARGET   , resource_name_c[2] },
    {RESOURCE_TYPE::COLLAGE  , resource_name_c[3] }
};

ProjectResource::~ProjectResource()
{
    imageLoader.forceStop();
    
    for (ofImage* & img : thumbnail_images) {
        delete(img);
    }
}

const vector<ofTexture>& ProjectResource::getThumbnails() const
{
    if (thumbnail_images.size() == thumbnail_textures.size()) {
        return thumbnail_textures;
    }
    else {
        thumbnail_textures.clear();

        for (ofImage* img : thumbnail_images) {
            if (img->isUsingTexture()) {
                thumbnail_textures.push_back(img->getTexture());
            }
        }
        return thumbnail_textures;
    }
}

bool ProjectResource::setup(const string & _path)
{
    dir = ofDirectory(_path);
    ofLogVerbose(__FUNCTION__) << "Loading: " << _path;
    if (dir.exists()) {
        for( auto & ext : get_allowed_exts())  dir.allowExt(ext);
        dir.listDir();

        switch (rt)
        {
        case ProjectResource::RESOURCE_TYPE::RAW:
        case ProjectResource::RESOURCE_TYPE::SEGMENTED:
        case ProjectResource::RESOURCE_TYPE::TARGET:
            loadThumbnails();
        default:
            break;

        }

        ofLogNotice() << dir.size() << " files found";
        return true;
    }
    else {
        ofLogWarning(__FUNCTION__) << "Could not open resource path: " << _path;
        return false;
    }
}

vector<string> ProjectResource::get_allowed_exts()
{
    switch (rt)
    {
    case RESOURCE_TYPE::RAW:
    case RESOURCE_TYPE::SEGMENTED:
    case RESOURCE_TYPE::TARGET:
        return LayerUtils::img_exts;
    case RESOURCE_TYPE::COLLAGE:
        return LayerUtils::collage_exts;
    default:
        return vector<string> { "" };
    }
}

void ProjectResource::loadThumbnails()
{
    for (const ofFile & file : dir) {
        thumbnail_images.push_back( new ofImage() );
        imageLoader.loadFromDisk(*thumbnail_images.back(), file.path(), THUMBNAIL_SIZE);
    }
}


//---------------------------------------------------------------------------------------


ProjectManager & ProjectManager::getInstance()
{
    static ProjectManager instance;
    return instance;
}

void ProjectManager::loadProject()
{
    LayerUtils::loadFolderDialogue(this, &ProjectManager::loadProject);
}

void ProjectManager::clear()
{
    b_isLoaded = false;
    thumbnails.clear();
    resources.clear();
    name = "";
}

const shared_ptr<ProjectResource> ProjectManager::getResource(RESOURCE_TYPE _rt) const
{
    auto resource = resources.find(_rt);
    if ( resource != resources.end()) {
        return resource->second;
    }
    else {
        return nullptr;
    }
}


void ProjectManager::loadProject(const string & _root_path)
{
    clear();

    root_path = _root_path;
    name = ofFilePath::getEnclosingDirectory(root_path);

    for (auto & resource_path : ProjectResource::resource_rel_paths) {
        RESOURCE_TYPE rt = resource_path.first;
        string path = ofFilePath::join(root_path, resource_path.second);
        shared_ptr<ProjectResource> newResource = make_shared<ProjectResource>(rt);
        if (newResource->setup(path)) {
            resources[resource_path.first] = newResource;
        }
    }

    if (resources.size() == 0) {
        ofLogWarning(__FUNCTION__) << "No resources found in project";
    }
    else {
        b_isLoaded = true;
    }

    onLoaded.notify(b_isLoaded);
}
