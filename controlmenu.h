#include <vector>
#include <string>
#include "frame.h"
#include "ioManager.h"
#include "vector2f.h"

class ControlMenu {
public:
  ControlMenu();
  ~ControlMenu();
  void draw() const;
  void lightOn() { click = ON; }
  void lightOff() { click = OFF; }
  void increment() { nextIcon = (nextIcon + 1) % controls.size(); }
  void decrement() { 
    nextIcon = (nextIcon + controls.size()-1) % controls.size(); 
  }
  void setMusic(bool m){
	  music = m;
  }
  bool getMusic() const{
	  return music;
  }
  void setCurMode(size_t m){
	  curMode = m;
  }
  size_t getCurMode() const{
	  return curMode;
  }
  const string& getIconClicked() const;
private:
  enum LightMode { OFF, ON };
  bool music;
  size_t curMode; 
  const IOManager& io;
  std::vector<std::string> controls; 

  ParseXML parser;
  Vector2f position;
  Vector2f space;
  SDL_Surface* surfaceOff;
  SDL_Surface* surfaceOn;
  std::vector<Frame> clicks;
  unsigned nextIcon;
  unsigned click;

  ControlMenu(const ControlMenu&);
  ControlMenu& operator=(const ControlMenu&);
};
