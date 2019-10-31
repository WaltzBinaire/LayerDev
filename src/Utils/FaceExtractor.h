#pragma once
#include "ofMain.h"


#ifdef NDEBUG
#include "ofxFaceTracker2.h"


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

    static ofxFaceTracker2 & getTracker();

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

#endif // !NDEBUG