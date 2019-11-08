#include "threadedImageLoader.h"



threadedImageLoader::threadedImageLoader(){
	nextID = 0;
    ofAddListener(ofEvents().update, this, &threadedImageLoader::update);

    startThread();
    lastUpdate = 0;
}

threadedImageLoader::~threadedImageLoader(){
    close();
}

// Load an image from disk.
//--------------------------------------------------------------
void threadedImageLoader::loadFromDisk(ofImage& image, string filename) {
		   
    nextID++;
	ofImageLoaderEntry entry(image);
	entry.filename = filename;
	entry.image->setUseTexture(false);
	entry.name = filename;
	entry.b_resize = false;
	images_to_load_from_disk.send(entry);
}

void threadedImageLoader::loadFromDisk(ofImage & image, string filename, int resize)
{
	nextID++;
	ofImageLoaderEntry entry(image);
	entry.filename = filename;
	entry.image->setUseTexture(false);
	entry.name = filename;
	entry.b_resize = true;
	entry.size = resize;
	images_to_load_from_disk.send(entry);
}

void threadedImageLoader::forceStop()
{
    close();
}


// Reads from the queue and loads new images.
//--------------------------------------------------------------
void threadedImageLoader::threadedFunction() {
	setThreadName("threadedImageLoader " + ofToString(thread.get_id()));
	ofImageLoaderEntry entry;
	while( images_to_load_from_disk.receive(entry) ) {

        if (!isThreadRunning()) close();

        if (entry.image != nullptr) {
            if(entry.image->load(entry.filename) )  {
			    images_to_update.send(entry);
		    }else{
			    ofLogError("threadedImageLoader") << "couldn't load file: \"" << entry.filename << "\"";
		    }
        }
        else {
			    ofLogError("threadedImageLoader") << "Image no longer exists: \"" << entry.filename << "\"";
        }

	}
	ofLogVerbose("threadedImageLoader") << "finishing thread on closed queue";
}


void threadedImageLoader::close()
{
    images_to_load_from_disk.close();
	images_to_update.close();
	waitForThread(true, INFINITE_JOIN_TIMEOUT);
    ofRemoveListener(ofEvents().update, this, &threadedImageLoader::update);
}

// Check the update queue and update the texture
//--------------------------------------------------------------
void threadedImageLoader::update(ofEventArgs & a){
    // Load 1 image per update so we don't block the gl thread for too long
	ofImageLoaderEntry entry;
    if (images_to_update.tryReceive(entry)) {

        if (entry.image != nullptr) {
            if (entry.b_resize) {
                glm::vec2 size;
                float aspect_ratio = entry.image->getWidth() / entry.image->getHeight();
                if (aspect_ratio > 1.0) {
                    size = glm::vec2(
                        entry.size,
                        entry.size / aspect_ratio
                    );
                }
                else {
                    size = glm::vec2(
                        entry.size  * aspect_ratio,
                        entry.size
                    );
                }
                entry.image->resize(size.x, size.y);
            }
            entry.image->setUseTexture(true);
            entry.image->update();
        }
        else {
			ofLogError("threadedImageLoader") << "Image no longer exists: \"" << entry.filename << "\"";
        }
    }
}