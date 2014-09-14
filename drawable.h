#ifndef DRAWABLE__H
#define DRAWABLE__H
#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include "vector2f.h"
#include "frame.h"

enum spritesType{ //My code, to record the orb's type
	SINGLE = 41,
	MULTI  = 42,
	FOURWAY = 43,
	PLAYER = 44,
	CHUNK = 45,
	OTHERTYPE = 46	
};

enum spritesDirection{ //My code, to record the orb's direction
	EAST = 23,
	WEST = 24,
	SOURTH = 25,
	NORTH = 26,
	OTHERDIR = 27
};

// Drawable is an Abstract Base Class (ABC) that
// specifies the methods that derived classes may
// and must have.
class Drawable {
public:
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel, const enum spritesType t, const enum spritesDirection d, double zx, double zy, double a): 
    name(n), position(pos), velocity(vel), type(t), direction(d), zoomX(zx), zoomY(zy), angle(a) {}

  Drawable(const Drawable& s) : 
    name(s.name), position(s.position), velocity(s.velocity), type(s.type), direction(s.direction), zoomX(s.zoomX), zoomY(s.zoomY), angle(s.angle)
    { }

  virtual ~Drawable() {}

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n;    }

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks, bool isPaused) = 0;
  virtual const Frame* getFrame() const = 0;
  virtual void zoom(double zoomX, double zoomY) = 0;
  virtual void rotate(double angle, double zoom) = 0;


  virtual void setWidth(int width) = 0;
  virtual void setHeight(int height) = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;

  float X() const { return position[0]; }
  void X(float x) { position[0] = x;    }

  float Y() const { return position[1]; }
  void Y(float y) { position[1] = y;    }

  float velocityX() const  { return velocity[0]; }
  void velocityX(float vx) { velocity[0] = vx;   }
  float velocityY() const  { return velocity[1]; }
  void velocityY(float vy) { velocity[1] = vy;   }

  const Vector2f& getVelocity() const   { return velocity; }
  void setVelocity(const Vector2f& vel) { velocity = vel;  }
  const Vector2f& getPosition() const   { return position; }
  void setPosition(const Vector2f& pos) { position = pos;  }
 
  enum spritesType getType() const { //My code
	  return type; 
  } 
  void setType(enum spritesType type){ //My code
  	this->type = type;
  }

  enum spritesDirection getDirection() const { //My code
  	return direction;
  }

  void setDirection(enum spritesDirection direction){ //My code
  	this->direction = direction;
  }
  void setZoom(double zx, double zy){
	  if(zx < 0.3){
		  zoomX = 0.3;
	  }else if(zx > 3.0){
	      zoomX = 3.0;
	  }else{
		  zoomX = zx;
	  }
	  if(zx < 0.3){
		  zoomY = 0.3;
	  }else if(zx > 3.0){
	      zoomY = 3.0;
	  }else{
		  zoomY = zy;
	  }
  }
  double getZoomX() const {
	  return zoomX;
  }
  double getZoomY() const {
	  return zoomY;
  }
  void setAngle(double a){
	  angle = a;
  }
  double getAngle() const {
	  return angle;
  }

  Drawable& operator=(const Drawable& da){
	  if(this == &da){
		  return *this;
	  }
	  name = da.name;
	  position = da.position;
	  velocity = da.velocity;
	  type = da.type;
	  direction = da.direction;
	  zoomX = da.zoomX;
	  zoomY = da.zoomY;
	  angle = da.angle;
	  return *this;
  }

private:
  std::string name;
  Vector2f position;
  Vector2f velocity;
  enum spritesType type;  //My code
  enum spritesDirection direction; //My code
  double zoomX, zoomY;
  double angle;
};
#endif
