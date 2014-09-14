#include "frameFactory.h"
#include "ioManager.h"
#include "vector2f.h"

FrameFactory::~FrameFactory() {
  std::map<std::string, Frame*>::iterator frame = frames.begin();
  while ( frame != frames.end() ) {
    delete frame->second;
    ++frame;
  }
  std::map<std::string, std::vector<const Frame*> >::iterator 
    frames = multiFrames.begin();
  while ( frames != multiFrames.end() ) {
    for (unsigned int i = 0; i < frames->second.size(); ++i) {
      delete frames->second[i];
    }
    ++frames;
  }
}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

Frame* FrameFactory::getFrame(const std::string& name) {
  std::map<std::string, Frame*>::const_iterator pos = frames.find(name); 
  if ( pos == frames.end() ) {
    Frame* frame = new Frame(name);
    frames[name] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}

std::vector<const Frame*> FrameFactory::getFrames(const std::string& name, enum spritesType type) {
  // First search map to see if we've already made it:
  std::map<std::string, std::vector<const Frame*> >::const_iterator 
    pos = multiFrames.find(name); 
  if ( pos != multiFrames.end() ) {
    return pos->second;
  }

  // It wasn't in the map, so we have to make the vector of Frames:
  SDL_Surface* surface = IOManager::
     getInstance().loadAndSet(gdata.getXmlStr(name+"File"), true);
  unsigned numberOfFrames = gdata.getXmlInt(name+"Frames");
  std::vector<const Frame*> frames;
  frames.reserve(numberOfFrames);
  Uint16 srcX = gdata.getXmlInt(name+"SrcX");
  Uint16 srcY = gdata.getXmlInt(name+"SrcY");
  Uint16 width = gdata.getXmlInt(name+"Width");
  Uint16 height = gdata.getXmlInt(name + "Height");
  int factor = gdata.getXmlInt(name + "Frames") / 4;

  for (unsigned i = 0; i < numberOfFrames; ++i) {
	unsigned int frameX = (i % factor) * width + srcX;
	if((type == PLAYER || type == FOURWAY) && i % factor ==0 && i != 0){ //For four-way sprites
		srcY += height;
	}
	frames.push_back( new Frame(surface, name, frameX, srcY) );
  }
  SDL_FreeSurface(surface);
  multiFrames[name] = frames;
  return frames;
}
