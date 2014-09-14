#include "vector2f.h"
#include "ioManager.h"
#include "aaline.h"

class Magic {
public:
  Magic();
  Magic(int sx, int sy, int tl, int cl, 
         float t, int inc, Uint32 c, float sp);
  void draw() const;
  void update(Uint32);
  void reset() { currentLength = totalLength; }
  void updateMagic(int hl){
	  if(hl > totalLength){
		  currentLength = totalLength;
	  }else{
	  	currentLength = hl;
	  }
  }
  int getMagic(){
	  return currentLength;
  }
  bool isFull(){
	  if(currentLength == totalLength){
		  return true;
	  }
	  return false;
  }
private:
  SDL_Surface* screen;
  Vector2f start;
  int totalLength;
  int currentLength;
  int thick;
  int increments;
  float interval;
  int deltaTime;
  const Uint32 RED;
  const Uint32 GRAY;
  const Uint32 BLACK;
  const Uint32 color;
  void drawBox() const;
  Magic(const Magic&);
  Magic& operator=(const Magic&);
};
