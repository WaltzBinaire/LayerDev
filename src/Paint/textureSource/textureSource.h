#pragma once
#include "ofMain.h"
#include "ofxSpout2Receiver.h"

class textureSourceBase
{
public:

    textureSourceBase();
    ~textureSourceBase();

    virtual ofTexture& getTexture() = 0;
    virtual void update() {};
};

class textureSourceSpout : public textureSourceBase
{
public:

    textureSourceSpout(string spoutName);

    ofTexture& getTexture() override;
    void update() override;

private:
    
    ofxSpout2::Receiver spoutReceiver; 
};

