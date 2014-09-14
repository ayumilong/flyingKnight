/*************************************************************************
    > File Name: planes.cpp
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Mon 18 Nov 2013 04:42:24 PM EST
 ************************************************************************/

#include <iostream>
#include <cmath>
#include "planes.h"
#include "gamedata.h"
#include "frameFactory.h"

Planes::Planes(const std::string& name, const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"X"), 
                    Gamedata::getInstance().getXmlInt(name+"Y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX"),  
                    Gamedata::getInstance().getXmlInt(name+"SpeedY")),
		   t, d, zx, zy, angle
           ),
  frame(FrameFactory::getInstance().getFrame(name)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  bullet()
  {
  }

Planes& Planes::operator=(const Planes& rhs) {
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

void Planes::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
  bullet.draw();
}

void Planes::update(const Vector2f& pos, Uint32 ticks, bool isPaused) {
	setPosition(pos);
	bullet.update(ticks, isPaused);
}

void Planes::shoot(const std::string& name, const Vector2f& vel, const Vector2f& incr){
	bullet.shoot(name, getPosition() + incr, vel, getDirection());
}
