#include <iostream>
#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"

Sprite::Sprite(const std::string& name, const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"X") + rand() % 4600, 
                    Gamedata::getInstance().getXmlInt(name+"Y") + rand() % 200), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX") + ((rand()%2 == 0) ? 1 : -1) * rand() % 200,  
                    Gamedata::getInstance().getXmlInt(name+"SpeedY")+ ((rand()%2 == 0) ? 1 : -1) * rand() % 200),
		   t, d, zx, zy, angle
           ),
  frame(FrameFactory::getInstance().getFrame(name)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
  {
  }

Sprite::Sprite(const std::string& n, const Vector2f& pos, const Vector2f& vel,  //Just for chunk uses
               const Frame* fm) :
  Drawable(n, pos, vel, CHUNK, NORTH, 1.0, 1.0, 0.0), 
  frame(fm),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s),
  frame(s.frame),  // shallow copy; Manager may reuse it
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
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
  frame = rhs.frame;  // shallow copy; Manager may reuse it
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

void Sprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

void Sprite::update(Uint32 ticks, bool isPaused) {
  if(isPaused){
  	return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if ( Y() < 0) {
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
