/*************************************************************************
    > File Name: multisprit.h
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: 2013年09月30日 星期一 18时49分30秒
 ************************************************************************/
#ifndef MULTISPRITE__H
#define MULTISPRITE__H

#include <string>
#include <vector>
#include <iostream>
#include "drawable.h"

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&, const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle);  
  MultiSprite(const MultiSprite& s); // canonical form
  virtual ~MultiSprite() { }  // Don't delete frame; factory will reuse it 
  MultiSprite& operator=(const MultiSprite&); // canonical form
  virtual void draw() const;
  virtual void update(Uint32 ticks, bool isPaused);
  virtual const std::vector<const Frame*> getFrames() const { return frames; };
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
  }
  virtual void rotate(double angle, double zoom){
	  std::size_t size = getSize();
	  while(size > 0){
		  --size;
  	      const_cast<Frame*>(frames[size]) -> rotate(angle, zoom);
	  }
	  resetWH(frames[0] -> getWidth(), frames[0] -> getHeight());
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
  void setWidth(int width){
	  frameWidth = width;
  }
  void setHeight(int height){
  	frameHeight = height;
  }

private:
  std::vector<const Frame*> frames;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  unsigned int number;
  int currentRV; //Current rotate velocity
};
#endif
