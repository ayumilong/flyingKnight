/*************************************************************************
    > File Name: bullet.cpp
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Mon 18 Nov 2013 05:14:12 PM EST
 ************************************************************************/

#include <iostream>
#include <cmath>
#include "bullet.h"
#include "gamedata.h"
#include "frameFactory.h"

Bullet::Bullet(const std::string& name, const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle, const Vector2f& pos, const Vector2f& vel) :
  Drawable(name, pos, vel, t, d, zx, zy, angle),
  frame(FrameFactory::getInstance().getFrame(name)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  initPosX(pos[0]),
  initPosY(pos[1])
  {
  }

Bullet::Bullet(const Bullet& s) :
  Drawable(s),
  frame(s.frame),  // shallow copy; Manager may reuse it
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  initPosX(s.initPosX),
  initPosY(s.initPosY)
{ }

Bullet& Bullet::operator=(const Bullet& rhs) {
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
  initPosX = rhs.initPosX;
  initPosY = rhs.initPosY;
  return *this;
}

void Bullet::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

void Bullet::update(Uint32 ticks, bool isPaused) {
  if(isPaused){
  	return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
}
