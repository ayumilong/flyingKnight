/*************************************************************************
    > File Name: player.cpp
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Mon 21 Oct 2013 07:56:33 PM EDT
 ************************************************************************/

#include <iostream>
#include "enemy.h"

Enemy::Enemy(const std::string& name, const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle): FourwaySprite(name, t, d, zx, zy, angle), blood(200), magic(200), jumpSpeed(200), jumpX(0), jumpY(0), jumping(false), peak(false), jumpLevel(1), upBullet(), downBullet(), leftBullet(), rightBullet(), cs(new PerPixelCollisionStrategy()) {
	jumpX = X();
	jumpY = Y();
	setPosition(Vector2f(random()%4500 + 1000, random()%280+ 60));
	setVelocity(Vector2f(-150, 0));
} 


Enemy& Enemy::operator=(const Enemy& p){
	if(this == &p){
		return *this;
	}
	FourwaySprite::operator=(p);
	blood = p.blood;
	magic = p.magic;
	jumpSpeed = p.jumpSpeed;
	jumpX = p.jumpX;
	jumpY = p.jumpY;
	jumping = p.jumping;
	peak = p.peak;
	jumpLevel = p.jumpLevel;
	return *this;
}

void Enemy::shoot(enum spritesDirection d){
	switch(d){
		case NORTH:
			upBullet.shoot("up", getPosition() + Vector2f(60, 0), Vector2f(0, -100 + velocityY()), d);
			break;
		case SOURTH:
			downBullet.shoot("down", getPosition() + Vector2f(60, 60), Vector2f(0, 100 + velocityY()), d);
			break;
		case WEST:
			leftBullet.shoot("left", getPosition() + Vector2f(20, 60), Vector2f(-100 + velocityX(), 0), d);
			break;
		case EAST:
			rightBullet.shoot("right", getPosition() + Vector2f(120, 60), Vector2f(100 + velocityX(), 0), d);
			break;
		default:
			break;
	}
}

bool Enemy::collisionDetect(const Drawable* d){
	return cs -> execute(*this, *d);
}
void Enemy::draw() const{
  	Uint32 x;
  	Uint32 y;
	if(jumping){
  		x = static_cast<Uint32>(jumpX);
  		y = static_cast<Uint32>(jumpY);
	}else{
  		x = static_cast<Uint32>(X());
  		y = static_cast<Uint32>(Y());
	}
  	getFrames()[getNumber()/getCurRV()] -> draw(x, y);
	upBullet.draw();
	downBullet.draw();
	leftBullet.draw();
	rightBullet.draw();
}

void Enemy::update(Uint32 ticks, bool isPaused){
	upBullet.update(ticks, isPaused);
	leftBullet.update(ticks, isPaused);
	rightBullet.update(ticks, isPaused);
	downBullet.update(ticks, isPaused);
  	unsigned int number = getNumber();
  	int currentRV = getCurRV();
  	std::size_t size = getSize(); //Frame size
	int factor = size / 4;
  	if(isPaused){
  		if(this -> getDirection() == SOURTH){ //For four-way sprites
  	    	number = 0 * currentRV;
  		}else if(this -> getDirection() == WEST){
  	    	number = 1 * factor * currentRV;
  		}else if(this -> getDirection() == EAST){
  	    	number = 2 * factor * currentRV;
  		}else if(this -> getDirection() == NORTH){
  	    	number = 3 * factor * currentRV;
  		}
		setNumber(number);
  	    return;
  	}
  	number = (number + 1) % (currentRV * size);
  	if(this -> getDirection() == SOURTH){ 
  	    while(number >= currentRV * size/4){
  	    	number -= currentRV * size/factor;
  	    }
  	}else if(this -> getDirection() == WEST){
  	    while(number < currentRV * size/4){
  	    	number += currentRV * size/factor;
  	    }
  	    while(number >= currentRV * size/2){
  	    	number -= currentRV * size/factor;
  	    }
  	}else if(this -> getDirection() == EAST){
  	    while(number < currentRV * size/2){
  	    	number += currentRV * size/factor;
  	    }
  	    while(number >= currentRV * size * 3/4){
  	    	number -= currentRV * size/factor;
  	    }
  	}else if(this -> getDirection() == NORTH){
  	    while(number < currentRV * size * 3/4){
  	    	number += currentRV * size/factor;
  	    }
  	}
	setNumber(number);

  	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	int worldWidth = getWorldWidth();
	//int worldHeight = getWorldHeight();
	int frameWidth = getWidth();
	//int frameHeight = getHeight();
	int yRange = getYRange();
	int xRange = getXRange();
  	setPosition(getPosition() + incr);
  	if (Y() < yRange){
		Y(yRange);
  	//}else if ( Y() > worldHeight - frameHeight){
	}else if(Y() > 400){
		Y(400);
  	}
  	if ( X() < xRange){
		X(xRange);
		velocityX(-velocityX());
		setDirection(EAST);
  	}else if ( X() > worldWidth - frameWidth){
		X(worldWidth - frameWidth);
		velocityX(-velocityX());
		setDirection(WEST);
  	}
	if(jumping){
		jumpX = X();
		jumpY = Y() - jumpSpeed;
	}
}

void Enemy::jump(){
	jumping = true;
	jumpX = X();
	jumpY = Y() - jumpSpeed;
}

void Enemy::stopJump(){
	jumping = false;
}

void Enemy::makeChange(enum spritesDirection newDir, enum spritesDirection counterDir, double vx, double vy){
	if(this -> getDirection() == counterDir){
		if(newDir == NORTH || newDir == SOURTH){
			if(newDir == NORTH){
				this -> velocityY(-abs(vy));
			}else{
				this -> velocityY(abs(vy));
			}
			this -> velocityX(0);
		}else{
			if(newDir == WEST){
				this -> velocityX(-abs(vx));
			}else{
				this -> velocityX(abs(vx));
			}
			this -> velocityY(0);
		}
	}else if(this -> getDirection() == newDir){
		if(newDir == NORTH){
			this -> velocityY(-abs(vy));
		}else if(newDir == SOURTH){
			this -> velocityY(abs(vy));
		}else if(newDir == WEST){
			this -> velocityX(-abs(vx));
		}else if(newDir == EAST){
			this -> velocityX(abs(vx));
		}
	}else{
		if(newDir == NORTH){
			this -> velocityY(-abs(vx));
			this -> velocityX(0);
		}else if(newDir == SOURTH){
			this -> velocityY(abs(vx));
			this -> velocityX(0);
		}else if(newDir == WEST){
			this -> velocityX(-abs(vy));
			this -> velocityY(0);
		}else if(newDir == EAST){
			this -> velocityX(abs(vy));
			this -> velocityY(0);
		}
	}
	this -> setDirection(newDir);
}
