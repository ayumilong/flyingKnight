/*************************************************************************
    > File Name: planes.h
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Mon 18 Nov 2013 04:42:20 PM EST
 ************************************************************************/

#ifndef PLANES__H 
#define PLANES__H

#include <string>
#include <iostream>
#include "drawable.h"
#include "bullets.h"

class Planes : public Drawable {
public:
  // In this version, frame is a parameter:
  Planes(const std::string&,const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle);  
  virtual ~Planes() { }  // Don't delete frame; factory will reuse it 
  Planes& operator=(const Planes&); // canonical form
  virtual void draw() const;
  virtual void update(Uint32 ticks, bool isPaused){ std::cout<<ticks<<isPaused<<std::endl;}
  void update(const Vector2f& pos, Uint32 ticks, bool isPaused);
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
  void shoot(const std::string& name, const Vector2f& vel, const Vector2f& incr);
  int getFreeCount(){
	  return bullet.getFreeCount();
  }
  int getBusyCount(){
	  return bullet.getBusyCount();
  }
  bool collideWith(const Drawable *d){
		return bullet.collisionDetect(d);
  }
private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  Bullets bullet; 

  int getDistance(const Planes*) const;
  Planes(const Planes& s); // canonical form
};
#endif
