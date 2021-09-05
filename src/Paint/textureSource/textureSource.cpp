#include "textureSource.h"



textureSourceBase::textureSourceBase()
{
}

textureSourceBase::~textureSourceBase()
{
}


//----------------------------------------------------------------------------
textureSourceSpout::textureSourceSpout(string spoutName) : textureSourceBase()
{
    //spoutReceiver.setup(spoutName);
}

ofTexture& textureSourceSpout::getTexture()
{
    //return spoutReceiver.getTexture();
    return ofTexture();
}

void textureSourceSpout::update()
{
    //spoutReceiver.updateTexture();
}