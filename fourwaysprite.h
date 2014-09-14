/*************************************************************************
    > File Name: fourwaysprite.h
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: 2013年10月01日 星期二 22时41分32秒
 ************************************************************************/

#ifndef FOURWAYSPRITE_H 
#define FOURWAYSPRITE_H 

#include <string>
#include <vector>
#include <iostream>
#include "sprite.h"

class FourwaySprite: public Sprite{
public:
  FourwaySprite(const std::string& name, const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle);  
  FourwaySprite(const FourwaySprite& s); // canonical form
  virtual ~FourwaySprite() { }  // Don't delete frame; factory will reuse it 
  FourwaySprite& operator=(const FourwaySprite&); // canonical form
  virtual void draw() const;
  virtual void update(Uint32 ticks, bool isPaused);
  virtual const std::vector<const Frame*>& getFrames() const { return frames; };
  virtual const Frame* getFrame() const { return frames[0]; };
  void resetWH(int w, int h){
	  frameWidth = w;
	  frameHeight = h;
  }
  virtual void zoom(double zoomX, double zoomY){
	  std::size_t size = getSize();
	  while(size > 0){
		  --size;
  	      const_cast<Frame*>(frames[size]) -> zoom(zoomX, zoomY);
	  }
	  resetWH(frames[0] -> getWidth(), frames[0] -> getHeight());
	  if(Y() + frameHeight > worldHeight){
	      Y( worldHeight - frameHeight); 
	  }else if(X() + frameWidth > worldWidth){
		  X(worldWidth - frameWidth);
	  }
  }
  virtual void rotate(double angle, double zoom){
	  angle = 0.0;
	  zoom = 1.0;  //These codes are just for no warning
  	  const_cast<Frame*>(frames[0]) -> rotate(angle, zoom); 
  }
  unsigned int getCurRV() const { return currentRV; }
  void setCurRV(int curRV) { 
	  if(curRV <= 0){
	  	currentRV = 1;
	  }else{
	  	currentRV = curRV;
	  }
  }
  unsigned int getNumber() const { return number; }
  void setNumber(unsigned int number) {
	  this -> number = number;
  }
  std::size_t getSize() const { return frames.size(); }
  
  int getWidth() const{
	  return frameWidth;
  }
  int getHeight() const{
	  return frameHeight;
  }

  int getWorldWidth() const {
	  return worldWidth;
  }
  int getWorldHeight() const {
	  return worldHeight;
  }
  void setWidth(int width){
	  frameWidth = width;
  }
  void setHeight(int height){
  	frameHeight = height;
  }

  void setRange(int xr, int yr){
	  xRange = xr;
	  yRange = yr;
  }
  int getXRange() const { return xRange; }
  int getYRange() const { return yRange; }

private:
  std::vector<const Frame*> frames;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  unsigned int number;
  int currentRV; //Current rotate velocity
  int xRange;
  int yRange;
  virtual void setVelAndDir(enum spritesDirection oldDir);
};
#endif
