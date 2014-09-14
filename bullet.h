/*************************************************************************
    > File Name: bullet.h
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Mon 18 Nov 2013 05:14:08 PM EST
 ************************************************************************/

#ifndef BULLET__H 
#define BULLET__H

#include <string>
#include <iostream>
#include "drawable.h"

const double DISTANCE = 500;

class Bullet : public Drawable {
public:
  // In this version, frame is a parameter:
  Bullet(const std::string&,const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle, const Vector2f& pos, const Vector2f& vef);  
  Bullet(const Bullet& s); // canonical form
  virtual ~Bullet() { }  // Don't delete frame; factory will reuse it 
  Bullet& operator=(const Bullet&); // canonical form
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

  bool goneTooFar(){
	  if(abs(X() - initPosX) > DISTANCE || abs(Y() - initPosY) > DISTANCE){
		  return true;
	  }else{
		  return false;
	  }
  }
  void setInitPosX(double ipx){
	  initPosX = ipx;
  }
  double getInitPosX(){
	  return initPosX;
  }
  void setInitPosY(double ipy){
	  initPosY = ipy;
  }
  double getInitPosY(){
	  return initPosY;
  }
private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  double initPosX;
  double initPosY;

  int getDistance(const Bullet*) const;
};
#endif
