#pragma once
#include "ofMain.h"

class threadedImageLoader : public ofThread {
public:
    threadedImageLoader();
    ~threadedImageLoader();

	void loadFromDisk(ofImage& image, string filename);
	void loadFromDisk(ofImage& image, string filename, int resize);

    void forceStop();

private:
    void close();
	void update(ofEventArgs & a);
    virtual void threadedFunction();
    
    // Entry to load.
    struct ofImageLoaderEntry {
        ofImageLoaderEntry() {
            image = NULL;
        }
        
        ofImageLoaderEntry(ofImage & pImage) {
            image = &pImage;
        }
        ofImage* image;
        string filename;
        string name;
        bool    b_resize;
        int     size;
    };


    typedef map<string, ofImageLoaderEntry>::iterator entry_iterator;

	int nextID;
    int lastUpdate;

	map<string,ofImageLoaderEntry> images_async_loading; // keeps track of images which are loading async
	ofThreadChannel<ofImageLoaderEntry> images_to_load_from_disk;
	ofThreadChannel<ofImageLoaderEntry> images_to_update;
};
