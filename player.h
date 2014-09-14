/*************************************************************************
    > File Name: player.h
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Mon 21 Oct 2013 07:56:27 PM EDT
 ************************************************************************/

#ifndef PLAYER_H 
#define PLAYER_H 

#include "fourwaysprite.h"
#include "clock.h"
#include "bullets.h"
#include "collisionStrategy.h"

class Player: public FourwaySprite{
public:
  Player(const std::string& name, const enum spritesType t, const enum spritesDirection d, double zx, double zy, double angle);  
  virtual ~Player() { delete cs; }  // Don't delete frame; factory will reuse it 
  Player& operator=(const Player& p); // canonical form
  virtual void draw() const;
  virtual void update(Uint32 ticks, bool isPaused);
  void jump();
  void stopJump();
  void goDown(){
	  makeChange(SOURTH, NORTH, 300, 300);
  }
  void goLeft(){
	  makeChange(WEST, EAST, 300, 300);
  }
  void goRight(){
	  makeChange(EAST, WEST, 300, 300);
  }
  void goUp(){
	  makeChange(NORTH, SOURTH, 300, 300);
  }
  void stop(){
	  setVelocity(Vector2f(0, 0));
  }
  void shoot(enum spritesDirection d);
  int getFreeCount(){
	  return upBullet.getFreeCount() + downBullet.getFreeCount() + leftBullet.getFreeCount() + rightBullet.getFreeCount();
  }
  int getBusyCount(){
	  return upBullet.getBusyCount() + downBullet.getBusyCount() + leftBullet.getBusyCount() + rightBullet.getBusyCount();
  }
  bool collideWith(const Drawable *d){
		return upBullet.collisionDetect(d) || downBullet.collisionDetect(d) || leftBullet.collisionDetect(d) || rightBullet.collisionDetect(d);
  }
  bool collisionDetect(const Drawable* d);
  void setBlood(int b) { blood = b; }
  void setMagic(int m) { magic = m; }
  int getBlood() const { return blood; }
  int getMagic() const { return magic; }
  void setJumping(bool b) { jumping = b; }
  bool getJumping() const { return jumping; }

private:
  int blood;
  int magic;
  int jumpSpeed;
  int jumpX;
  int jumpY;
  bool jumping;
  bool peak;
  int jumpLevel;
  Bullets upBullet;
  Bullets downBullet;
  Bullets leftBullet;
  Bullets rightBullet;
CollisionStrategy* cs; 
  Player(const Player& p); // canonical form

  void makeChange(enum spritesDirection newDir, enum spritesDirection counterDir, double vx, double vy);
};
#endif
