#ifndef SPRITE__H
#define SPRITE__H

#include <string>
#include <iostream>
#include "drawable.h"

class Sprite : public Drawable {
public:
  // In this version, frame is a parameter:
  Sprite(const std::string&,const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle);  
  Sprite(const std::string& n, const Vector2f& pos, const Vector2f& vel, const Frame* fm);
  Sprite(const Sprite& s); // canonical form
  virtual ~Sprite() { }  // Don't delete frame; factory will reuse it 
  Sprite& operator=(const Sprite&); // canonical form
  virtual void draw() const;
  virtual void update(Uint32 ticks, bool isPaused);
  virtual const Frame* getFrame() const { return frame; }
  void resetWH(int w, int h){
	  frameWidth = w;
	  frameHeight = h;
  }
  virtual void zoom(double zoomX, double zoomY){
  	const_cast<Frame*>(frame) -> zoom(zoomX, zoomY);
	resetWH(frame -> getWidth(), frame -> getHeight());
  }
  virtual void rotate(double angle, double zoom){
  	const_cast<Frame*>(frame) -> rotate(angle, zoom);
	resetWH(frame -> getWidth(), frame -> getHeight());
  }

  int getWidth() const{
	  return frameWidth;
  }
  int getHeight() const{
	  return frameHeight;
  }
  void setWidth(int width){
	  frameWidth = width;
  }
  void setHeight(int height){
  	frameHeight = height;
  }
private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const Sprite*) const;
};
#endif
