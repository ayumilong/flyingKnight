#include "SDL/SDL_rotozoom.h"
#include "ioManager.h"
#include "frame.h"
#include "extractSurface.h"
#include "viewport.h"

// The next constructor s/ only be used when the Frames's surface
// starts at (0, 0) in the sprite sheet, and consumes the entire
// sprite sheet. For example, a Frame containing a background.
Frame::Frame( const std::string& name ) : 
  name(name),
  screen(IOManager::getInstance().getScreen()),
  originalWidth(Gamedata::getInstance().getXmlInt(name+"Width")), 
  originalHeight(Gamedata::getInstance().getXmlInt(name+"Height")),
  width(originalWidth),
  height(originalHeight),
  originalSurface( IOManager::getInstance().
           loadAndSet(Gamedata::getInstance().getXmlStr(name+"File"), 
           Gamedata::getInstance().getXmlBool(name+"Transparency") ) ),
  surface(originalSurface),
	sourceX(0),
	sourceY(0)
{ }

// The next constructor gets all parameters from surf.
Frame::Frame( SDL_Surface* surf ) :
  name(),
  screen(IOManager::getInstance().getScreen()),
  originalWidth(surf->w), 
  originalHeight(surf->h),
  width(originalWidth),
  height(originalHeight),
  originalSurface(surf),
  surface(originalSurface),
	sourceX(0),
	sourceY(0)
{}

// The next constructor s/ be used when the Frame's surface is
// found on only a part of sprite sheet, width and height is in XML:
Frame::Frame( SDL_Surface* surf, const std::string& name,
              Sint16 src_x, Sint16 src_y) :
  name(name),
  screen(IOManager::getInstance().getScreen()),
  originalWidth(Gamedata::getInstance().getXmlInt(name+"Width")), 
  originalHeight(Gamedata::getInstance().getXmlInt(name+"Height")),
  width(originalWidth),
  height(originalHeight),
  originalSurface( ExtractSurface::getInstance().
    get(surf, width, height, src_x, src_y) 
  ),
  surface(originalSurface),
	sourceX(src_x),
	sourceY(src_y)
{ }

// The next constructor s/ be used when the Frame's surface is
// found on only a part of sprite sheet, width and height is passed:
Frame::Frame( SDL_Surface* surf, Sint16 w, Sint16 h, 
              Sint16 src_x, Sint16 src_y) :
  name(),
  screen(IOManager::getInstance().getScreen()),
  originalWidth(w),
  originalHeight(h),
  width(originalWidth),
  height(originalHeight),
  originalSurface( ExtractSurface::getInstance().
    get(surf, width, height, src_x, src_y) 
  ),
  surface(originalSurface),
	sourceX(src_x),
	sourceY(src_y)
{ }

Frame::Frame( const Frame& frame ) :
  name(frame.name),
  screen(frame.screen),
  originalWidth(frame.width), originalHeight(frame.height),
  width(originalWidth),
  height(originalHeight),
  // Need a deep copy here:
  originalSurface( 
    ExtractSurface::getInstance().get(frame.surface, width, height, 0, 0) 
  ),
  surface(originalSurface),
	sourceX(frame.sourceX),
	sourceY(frame.sourceY)
{ }

Frame& Frame::operator=(const Frame& rhs) {
  name = rhs.name;
  screen = rhs.screen;
  originalWidth = rhs.originalWidth;
  originalHeight = rhs.originalHeight;
  width = rhs.width;
  height = rhs.height;
  originalSurface = rhs.originalSurface;
  surface = rhs.surface;
  sourceX = rhs.sourceX;
  sourceY = rhs.sourceY;
  return *this;
}

void Frame::draw(Sint16 x, Sint16 y) const {
  x -= Viewport::getInstance().X();
  y -= Viewport::getInstance().Y();
  SDL_Rect src = { 0, 0, width, height };    
  SDL_Rect dest = {x, y, width, height };
  SDL_BlitSurface(surface, &src, screen, &dest);
}

void Frame::draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const {
  SDL_Rect src = { sx, sy, width, height };    
  SDL_Rect dest = {dx, dy, width, height };
  SDL_BlitSurface(surface, &src, screen, &dest);
}

void Frame::draw(Sint16 x, Sint16 y, double angle) const {
  SDL_Surface* tmp = rotozoomSurface(surface, angle, 1, 1);
  SDL_Rect src = { 0, 0, tmp->w, tmp->h };    
  SDL_Rect dest = {x, y, 0, 0 };
  SDL_BlitSurface(tmp, &src, screen, &dest);
  SDL_FreeSurface( tmp );
}
