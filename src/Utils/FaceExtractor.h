#pragma once
#include "ofMain.h"

//#define USE_DLIB

#ifdef DEBUG
#undef USE_DLIB
#endif

#ifdef USE_DLIB
#include "ofxFaceTracker2.h"
#else
#include "ofxCv.h"
#endif // !NDEBUG

class FaceExtractor
{
public:
    static FaceExtractor& getInstance();

    void extractFaces(const string & in_path, const string & out_path);

    float getProgress() const {
        if (b_isRunning) {            
            return (float)distance(in_dir.begin(), currentFile) / in_dir.size();
        }
        else {
            return 1.0;
        }
    };

    bool isRunning() const {
        return b_isRunning;
    }

    ofEvent<bool> faceExtractorComplete;

    FaceExtractor (FaceExtractor const&)  = delete;
    void operator=(FaceExtractor const&)  = delete;

private:

    FaceExtractor();
    ~FaceExtractor();

#ifdef USE_DLIB
    static ofxFaceTracker2 & getTracker();
#else
    ofxCv::ObjectFinder tracker;
#endif

    void start();
    void onUpdate(ofEventArgs & _args);
    void stop();

    void extractNextFace();

    bool b_isRunning;
    ofDirectory in_dir;
    ofDirectory out_dir;

    vector<ofFile>::const_iterator currentFile;
    int saveNum;

    ofEventListener l_onUpdate;
};

