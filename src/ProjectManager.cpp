#include "ProjectManager.h"

const map<ProjectResource::RESOURCE_TYPE, string> ProjectResource::resource_rel_paths
{
    {RESOURCE_TYPE::RAW      , "Assets/Raw"       },
    {RESOURCE_TYPE::SEGMENTED, "Assets/Segmented" },
    {RESOURCE_TYPE::TARGET   , "Assets/Targets"   },
    {RESOURCE_TYPE::COLLAGE  , "Assets/Collage"   },
    {RESOURCE_TYPE::MASKS    , "Assets/Masks"     },
    {RESOURCE_TYPE::FACES    , "Assets/Faces"     }
};

const char* ProjectResource::resource_name_c[] = {
    "Instagram",
    "Segmented",
    "Portrait" ,
    "Collage"  , 
    "Masks"    ,
    "Faces"    
};

const map<ProjectResource::RESOURCE_TYPE, string> ProjectResource::resource_names
{
    {RESOURCE_TYPE::RAW      , resource_name_c[0] },
    {RESOURCE_TYPE::SEGMENTED, resource_name_c[1] },
    {RESOURCE_TYPE::TARGET   , resource_name_c[2] },
    {RESOURCE_TYPE::COLLAGE  , resource_name_c[3] },
    {RESOURCE_TYPE::MASKS    , resource_name_c[4] },
    {RESOURCE_TYPE::FACES    , resource_name_c[5] }
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
    rootDir = ofDirectory(_path);
    ofLogVerbose(__FUNCTION__) << "Loading: " << _path;
    if (rootDir.exists()) {

        bool recursiveSearch = rt == RESOURCE_TYPE::COLLAGE;

        if (recursiveSearch) {
            scanDir(rootDir, get_allowed_exts());
        }
        else {
            for( auto & ext : get_allowed_exts())  rootDir.allowExt(ext);
            rootDir.listDir();

            filePaths.clear();
            for (const auto & file : rootDir) filePaths.push_back(file.path());
        }

        switch (rt)
        {
        case ProjectResource::RESOURCE_TYPE::RAW:
        case ProjectResource::RESOURCE_TYPE::SEGMENTED:
        case ProjectResource::RESOURCE_TYPE::TARGET:
        case ProjectResource::RESOURCE_TYPE::MASKS:
        case ProjectResource::RESOURCE_TYPE::FACES:
            loadThumbnails();
            break;
        case RESOURCE_TYPE::COLLAGE:
            loadCollageThumbnails();
            break;
        default:
            break;

        }

        ofLogNotice() << filePaths.size() << " files found";
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
    case RESOURCE_TYPE::MASKS:
    case RESOURCE_TYPE::FACES:
        return LayerUtils::img_exts;
    case RESOURCE_TYPE::COLLAGE:
        return LayerUtils::collage_exts;
    default:
        return vector<string> { "" };
    }
}

inline void ProjectResource::scanDir(ofDirectory & dir, const vector<string>& allowedExts)
{
    dir.listDir();

    for (auto file : dir)
    {
        string ext = file.getExtension();

        auto result = find(allowedExts.begin(), allowedExts.end(), ext);

        if (result != allowedExts.end()) {
            filePaths.push_back(file.path());
        }
        else if (file.isDirectory())
        {
            scanDir(ofDirectory(file.getAbsolutePath()), allowedExts);
        }
    }
}

void ProjectResource::loadThumbnails()
{
    for (const string & file : filePaths) {
        thumbnail_images.push_back( new ofImage() );
        imageLoader.loadFromDisk(*thumbnail_images.back(), file, THUMBNAIL_SIZE);
    }
}

void ProjectResource::loadCollageThumbnails()
{
    for (const string & file : filePaths) {

        string imagePath(file);

        if (imagePath.size() > 5) {
            imagePath.replace(imagePath.end()-3, imagePath.end(), "png");

            ofFile imageFile(imagePath);
            if (imageFile.exists()) {
                ofLogNotice(__FUNCTION__) << imagePath;
                thumbnail_images.push_back( new ofImage() );
                imageLoader.loadFromDisk(*thumbnail_images.back(), imagePath, THUMBNAIL_SIZE);
            }
        }
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

void ProjectManager::populateFaceFolder()
{
#ifdef NDEBUG
    auto raw = getResource(RESOURCE_TYPE::RAW);

    if (raw == nullptr) {
        ofLogWarning(__FUNCTION__) << "Raw image folder not found";
        return;
    }

    FaceExtractor & faceExtractor = FaceExtractor::getInstance();

    string in_path  = ofFilePath::join( root_path , ProjectResource::resource_rel_paths.at(RESOURCE_TYPE::RAW));
    string out_path = ofFilePath::join( root_path , ProjectResource::resource_rel_paths.at(RESOURCE_TYPE::FACES));

    faceExtractor.extractFaces( in_path, out_path );


    l_onFaceExtractorComplete = faceExtractor.faceExtractorComplete.newListener( [this](bool &) {
        this->createResource(RESOURCE_TYPE::FACES, ProjectResource::resource_rel_paths.at(RESOURCE_TYPE::FACES) );
        this->l_onFaceExtractorComplete.unsubscribe();
        }
    );
#endif // !NDEBUG
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
        createResource(resource_path.first, resource_path.second );
    }

    if (resources.size() == 0) {
        ofLogWarning(__FUNCTION__) << "No resources found in project";
    }
    else {
        b_isLoaded = true;
    }

    onLoaded.notify(b_isLoaded);
}

void ProjectManager::createResource( ProjectResource::RESOURCE_TYPE rt, const string & resource_path)
{
    string path = ofFilePath::join(root_path, resource_path);
    shared_ptr<ProjectResource> newResource = make_shared<ProjectResource>(rt);
    if (newResource->setup(path)) {
        resources[rt] = newResource;
    }
}
