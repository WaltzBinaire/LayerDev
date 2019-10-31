#include "Utils\FaceExtractor.h"
#include "LayerUtils.h"

#ifdef NDEBUG
//---------------------------------------------------------------------------------------
FaceExtractor & FaceExtractor::getInstance()
{
    static FaceExtractor instance;
    return instance;
}


void FaceExtractor::extractFaces(const string & in_path, const string & out_path)
{
    if (b_isRunning) {
        ofLogError(__FUNCTION__) << "FaceExtractor already running";
    }

    in_dir = ofDirectory (in_path);
    if (!in_dir.exists()) {
        ofLogError(__FUNCDNAME__) << "Directory does not exist: " << in_path;
        return;
    }

    out_dir = ofDirectory(out_path);


    for( auto & ext : LayerUtils::img_exts)  in_dir.allowExt(ext);
    in_dir.listDir();

    if (in_dir.size() == 0) {
        ofLogWarning(__FUNCDNAME__) << "No images found in: " << in_path;
    }
    else {
        ofLogNotice(__FUNCDNAME__) << in_dir.size() << " images found";
    }

    if (!out_dir.exists()) {
        out_dir.create(true);
        ofLogNotice(__FUNCDNAME__) << "Creating directory: " << out_path;
    }

    currentFile = in_dir.begin();
    saveNum = 0;
    start();
}

FaceExtractor::FaceExtractor() :
    b_isRunning(false)
{}

FaceExtractor::~FaceExtractor() {
    FaceExtractor::getTracker().stop();
}

void FaceExtractor::start()
{
    ofLogVerbose(__FUNCTION__) << "Starting Face Extractor";
    b_isRunning = true;
    l_onUpdate = ofEvents().update.newListener(this, &FaceExtractor::onUpdate);
}

void FaceExtractor::onUpdate(ofEventArgs & _args)
{
    extractNextFace();
}

void FaceExtractor::stop()
{
    l_onUpdate.unsubscribe();
    b_isRunning = false;
    in_dir.close();
    out_dir.close();
}

void FaceExtractor::extractNextFace()
{
    static ofImage img;
    img.setUseTexture(false);
    
    ofLogVerbose(__FUNCTION__) << "Loading: " << currentFile->path() << " " << (100 * getProgress()) << "%";

    img.load(currentFile->path());
    if (img.isAllocated()) {
        ofxFaceTracker2 & tracker = getTracker();
        tracker.update(img.getPixels());

        for (auto instance : tracker.getInstances()) {
            ofImage faceInstance;
            ofRectangle rect = instance.getBoundingBox();

            faceInstance.cropFrom(img, rect.x, rect.y, rect.width, rect.height);

            string fileName = "Face_" + ofToString(saveNum++) + ".png";
            string path = ofFilePath::join(out_dir.path(), fileName);
            faceInstance.save(path);
        }
    }

    currentFile++;
    if (currentFile == in_dir.end()) {
        ofLogNotice(__FUNCTION__) << "All files processed";
        stop();

        bool success = true;
        faceExtractorComplete.notify(success);
    };
}

ofxFaceTracker2 & FaceExtractor::getTracker() {
    static bool isSetup = false;
    static ofxFaceTracker2 tracker;
    if (!isSetup) {
        tracker.setup(); 
        tracker.setThreaded(false);
        isSetup = true;
    }
    return tracker;
}
#endif // !NDEBUG