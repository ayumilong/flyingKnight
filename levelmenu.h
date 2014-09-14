#include <vector>
#include <string>
#include "frame.h"
#include "ioManager.h"
#include "vector2f.h"

class LevelMenu {
public:
  LevelMenu();
  ~LevelMenu();
  void draw() const;
  void lightOn() { click = ON; }
  void lightOff() { click = OFF; }
  void increment() { nextIcon = (nextIcon + 1) % levels.size(); }
  void decrement() { 
    nextIcon = (nextIcon + levels.size()-1) % levels.size(); 
  }
  int getLevel() const{
	  return nextIcon;
  }
  void setLevel(int l){
	  nextIcon = l;
  }
  const string& getIconClicked() const;
private:
  enum LightMode { OFF, ON };
  const IOManager& io;
  std::vector<SDL_Surface*> levelSurface;
  std::vector<Frame> levels; 

  ParseXML parser;
  Vector2f position;
  Vector2f space;
  SDL_Surface* surfaceOff;
  SDL_Surface* surfaceOn;
  std::vector<Frame> clicks;
  unsigned nextIcon;
  unsigned click;

  LevelMenu(const LevelMenu&);
  LevelMenu& operator=(const LevelMenu&);
};
