#include "LayerUtils.h"


namespace LayerUtils {
    bool saveFolderDialogue(string & path)
    {
        ProjectManager & projectManager = ProjectManager::getInstance();
        
        string defaultPath = "";
        if (projectManager.isLoaded()) defaultPath = projectManager.getPath();

        auto result = ofSystemLoadDialog("Open Folder...", true, defaultPath );

        if (result.bSuccess) {
            path = result.getPath();
            return true;
        }
        return false;
    }
    bool saveImageDialogue(string & path)
    {
        auto result = ofSystemSaveDialog("image", "Save as..." );

        if (result.bSuccess) {
            string ext      = ofFilePath::getFileExt(result.filePath);
            string baseName = ofFilePath::getBaseName(result.filePath);
            string dir      = ofFilePath::getEnclosingDirectory(result.filePath);

            if (find(img_exts.begin(), img_exts.end(), ext) != img_exts.end()) {
                path = result.filePath;
            }
            else {
                path = ofFilePath::join(dir, baseName + ".png");
            }
            return true;
        }
        else {
            return false;
        }
    }
    bool saveImage(const string path, const ofImage & image)
    {
        return image.save(path, OF_IMAGE_QUALITY_BEST);
    }
    bool saveImage(const string path, const ofTexture & texture)
    {
        ofPixels pixels;
        texture.readToPixels(pixels);
        return saveImage(path, pixels);
    }
    bool saveImage(const string path, const ofPixels & pixels)
    {
        ofImage img(pixels);
        return saveImage(path, img);
    }
    bool saveImage(const string path, const ofFbo & fbo)
    {
        return saveImage(path, fbo.getTexture());
    }
    bool saveImage(const string path, const pingPongFbo & fbo)
    {
        return saveImage(path, fbo.getTexture());
    }
}