#ifndef FRAME__H
#define FRAME__H

#include <iostream>
#include <string>
#include <SDL.h>
#include "SDL/SDL_rotozoom.h"

class Frame {
public:
  // The next constructor gets all parameters from the XML file.
  Frame( const std::string& name );

  // The next constructor gets all parameters from surf.
  Frame( SDL_Surface* surf );

  // When using the next constructor, we pass src_x, and src_y,
  // and we get the frame width and height from XML.
  // We do this because the sprite is only part of the sprite sheet;
  // also, multi-frame sprites may start at different src_x or src_y.
  Frame( SDL_Surface* surf, const std::string& name, 
         Sint16 src_x, Sint16 src_y);

  // When using the next constructor, we pass src_x, and src_y,
  // width, and height; usually used for scaled or rotating frames:
  Frame( SDL_Surface* surf, Sint16 src_x, Sint16 src_y, Sint16 w, Sint16 h);

  Frame(const Frame&);
  Frame& operator=(const Frame&);
  ~Frame() { 
	  if(surface != originalSurface){
	  	SDL_FreeSurface( surface ); 
	 	SDL_FreeSurface( originalSurface );
	  }else{
	 	SDL_FreeSurface( originalSurface );
	  }	
  }

  SDL_Surface* getSurface() const { return surface; }
  void draw(Sint16 x, Sint16 y) const;
  void draw(Sint16 x, Sint16 y, double angle) const;
  void draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const;

  Uint16 getWidth()  const { return width; }
  Uint16 getHeight() const { return height; }
  const std::string& getName() const { return name; }
  void setName(const std::string& name){ this -> name = name; }

  void rotate(double angle, double zoom){
	  surface = rotozoomSurface(originalSurface, angle, zoom, SMOOTHING_ON);
	  width = surface -> w;
	  height = surface -> h;
  }

  void zoom(double zoomX, double zoomY){
	  surface = zoomSurface(originalSurface, zoomX, zoomY, SMOOTHING_ON);
	  width = surface -> w;
	  height = surface -> h;
  }
  Uint16 getSourceX() const { return sourceX; }
  Uint16 getSourceY() const { return sourceY; }
private:
  std::string name;
  SDL_Surface * screen;
  Uint16 originalWidth;
  Uint16 originalHeight;
  Uint16 width;
  Uint16 height;
  SDL_Surface *originalSurface;
  SDL_Surface * surface;
  Sint16 sourceX;
  Sint16 sourceY;

  Frame();
};

#endif
