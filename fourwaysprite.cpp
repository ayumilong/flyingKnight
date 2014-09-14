/*************************************************************************
    > File Name: fourwaysprite.cpp
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: 2013年10月01日 星期二 22时41分24秒
 ************************************************************************/

#include <iostream>
#include <vector>
#include "fourwaysprite.h"
#include "gamedata.h"
#include "frameFactory.h"

FourwaySprite::FourwaySprite(const std::string& name,  const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle):
	Sprite(name, 
		   t, d, zx, zy, angle 
           ), 
	frames(FrameFactory::getInstance().getFrames(name, t)),	
	frameWidth(Gamedata::getInstance().getXmlInt(name + "Width")),
  	frameHeight(Gamedata::getInstance().getXmlInt(name + "Height")),
  	worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  	worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
	number(0),
	currentRV(Gamedata::getInstance().getXmlInt(name + "DefaultRV")),
	xRange(Gamedata::getInstance().getXmlInt("level1XRange")),
	yRange(Gamedata::getInstance().getXmlInt("level1YRange"))
{
	if(currentRV <= 0){ //The minimum value for currentRV is 1
		currentRV = 1;
	}
}

FourwaySprite::FourwaySprite(const FourwaySprite& s) :
  Sprite(s),
  frames(s.getFrames()),
  frameWidth(Gamedata::getInstance().getXmlInt(s.getName() + "Width")),
  frameHeight(Gamedata::getInstance().getXmlInt(s.getName() + "Height")),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),
  number(s.number),
  currentRV(s.currentRV),
  xRange(s.xRange),
  yRange(s.yRange)
{ 
}

FourwaySprite& FourwaySprite::operator=(const FourwaySprite& rhs) {
  if(this == &rhs){
  	return *this;
  }
  Sprite::operator=(rhs); //
  frames = rhs.getFrames();
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  number = rhs.number;
  currentRV = rhs.currentRV;
  xRange = rhs.xRange;
  yRange = rhs.yRange;
  return *this;
}

void FourwaySprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[number/currentRV] -> draw(x, y);
}

void FourwaySprite::update(Uint32 ticks, bool isPaused) {
  if((velocityX() == 0 && velocityY() == 0) || isPaused){
  	if(this -> getDirection() == SOURTH){ //For four-way sprites
	  	number = 0 * currentRV;
  	}else if(this -> getDirection() == WEST){
	  	number = 4 * currentRV;
  	}else if(this -> getDirection() == EAST){
	  	number = 8 * currentRV;
  	}else if(this -> getDirection() == NORTH){
	  	number = 12 * currentRV;
  	}
	  return;
  }
  std::size_t size = frames.size();
  number = (number + 1) % (currentRV * size);
  if(this -> getDirection() == SOURTH){ //For four-way sprites
	  while(number >= currentRV * size/4){
	  	number -= currentRV * size/4;
	  }
  }else if(this -> getDirection() == WEST){
	  while(number < currentRV * size/4){
	  	number += currentRV * size/4;
	  }
	  while(number >= currentRV * size/2){
	  	number -= currentRV * size/4;
	  }
  }else if(this -> getDirection() == EAST){
	  while(number < currentRV * size/2){
	  	number += currentRV * size/4;
	  }
	  while(number >= currentRV * size * 3/4){
	  	number -= currentRV * size/4;
	  }
  }else if(this -> getDirection() == NORTH){
	  while(number < currentRV * size * 3/4){
	  	number += currentRV * size/4;
	  }
  }

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  if (Y() < yRange){
	  setVelAndDir(getDirection());
  }else if ( Y() > 400){
	  setVelAndDir(getDirection());
  }
  if ( X() < xRange){
	  setVelAndDir(getDirection());
  }else if ( X() > worldWidth-frameWidth){
	  setVelAndDir(getDirection());
  }
}

void FourwaySprite::setVelAndDir(enum spritesDirection oldDir){
	enum spritesDirection newDir = oldDir;
	while(newDir == oldDir){
  		unsigned int pos = rand() % 4;
  		enum spritesDirection sd[] = {EAST, SOURTH, WEST, NORTH};
		newDir = sd[pos];
	}
	if(oldDir == SOURTH || oldDir == NORTH){ //The sprites is going to SOURTH
		if(newDir == WEST){
			velocityX(-abs(velocityY()));
			velocityY(0);
			setDirection(newDir);
		}else if(newDir == EAST){
			velocityX(abs(velocityY()));
			velocityY(0);
			setDirection(newDir);
		}else{
			if(oldDir == SOURTH){
				setDirection(NORTH);
				velocityY(-abs(velocityY()));
			}else{
				setDirection(SOURTH);
				velocityY(abs(velocityY()));
			}
		}
		if(oldDir == NORTH){
			Y(yRange);
		}else{
			Y(400);
		}
	}else if(oldDir == EAST || oldDir == WEST){
		if(newDir == SOURTH){
			velocityY(abs(velocityX()));
			velocityX(0);
			setDirection(newDir);
		}else if(newDir == NORTH){
			velocityY(-abs(velocityX()));
			velocityX(0);
			setDirection(newDir);
		}else{
			if(oldDir == WEST){
				setDirection(EAST);
				velocityX(abs(velocityX()));
			}else{
				setDirection(WEST);
				velocityX(-abs(velocityX()));
			}
		}
		if(oldDir == WEST){
			X(xRange);
		}else{
			X(worldWidth- frameWidth);
		}
	}
}
