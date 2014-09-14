/*************************************************************************
    > File Name: multisprit.cpp
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: 2013年09月30日 星期一 18时49分22秒
 ************************************************************************/

#include <iostream>
#include <vector>
#include "multisprite.h"
#include "gamedata.h"
#include "frameFactory.h"

MultiSprite::MultiSprite(const std::string& name,  const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle):
	Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"X") + ((rand()%2 == 1) ? 1 : -1) * rand() % 2000, 
                    Gamedata::getInstance().getXmlInt(name+"Y") + ((rand()%2 == 1) ? 1 : -1) * rand() % 600), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX") + ((rand()%2 == 0) ? 1 : -1) * rand() % 200,  
                    Gamedata::getInstance().getXmlInt(name+"SpeedY")+ ((rand()%2 == 0) ? 1 : -1) * rand() % 200),
		   t, d, zx, zy, angle
           ), 
	frames(FrameFactory::getInstance().getFrames(name, t)),	
	frameWidth(Gamedata::getInstance().getXmlInt(name + "Width")),
  	frameHeight(Gamedata::getInstance().getXmlInt(name + "Height")),
  	worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  	worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
	number(0),
	currentRV(Gamedata::getInstance().getXmlInt(name + "DefaultRV"))
{
	if(currentRV <= 0){ //The minimum value for currentRV is 1
		currentRV = 1;
	}
}

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity(), s.getType(), s.getDirection(), s.getZoomX(), s.getZoomY(), s.getAngle()), 
  frames(s.getFrames()),
  frameWidth(Gamedata::getInstance().getXmlInt(s.getName() + "Width")),
  frameHeight(Gamedata::getInstance().getXmlInt(s.getName() + "Height")),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  number(s.number),
  currentRV(s.currentRV)
{ 
}

MultiSprite& MultiSprite::operator=(const MultiSprite& rhs) {
  if(this == &rhs){
  	return *this;
  }
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  setType(rhs.getType());
  setDirection(rhs.getDirection()); 
  setZoom(rhs.getZoomX(), rhs.getZoomY());
  setAngle(rhs.getAngle());
  frames = rhs.getFrames();
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  number = rhs.number;
  currentRV = rhs.currentRV;
  return *this;
}

void MultiSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[number/currentRV] -> draw(x, y);
}

void MultiSprite::update(Uint32 ticks, bool isPaused) {
  if(isPaused || (velocityX() == 0 && velocityY() == 0)){
  	return;
  }
  std::size_t size = frames.size();
  number = (number + 1) % (currentRV * size);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( Y() < 0){ 
   	velocityY( abs( velocityY() ) );
	Y(0);
  }else if ( Y() > 200) {
   	velocityY( -abs( velocityY() ) );
	Y(200);
  }
  if ( X() < 0) {
  	velocityX( abs( velocityX() ) );
	X(0);
  }else if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
	X(worldWidth-frameWidth);
  }
}
