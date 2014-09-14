#include <vector>
#include <string>
#include "frame.h"
#include "ioManager.h"
#include "vector2f.h"

class Menu {
public:
  Menu(const std::string&);
  ~Menu();
  void draw() const;
  void lightOn() { click = ON; }
  void lightOff() { click = OFF; }
  void increment() { nextIcon = (nextIcon + 1) % words.size(); }
  void decrement() { 
    nextIcon = (nextIcon + words.size()-1) % words.size(); 
  }
  const string& getIconClicked() const;
private:
  enum LightMode { OFF, ON };
  const IOManager& io;
  std::vector<std::string> words; 

  ParseXML parser;
  Vector2f position;
  Vector2f space;
  SDL_Surface* surfaceOff;
  SDL_Surface* surfaceOn;
  std::vector<Frame> clicks;
  unsigned nextIcon;
  enum LightMode click;

  Menu(const Menu&);
  Menu& operator=(const Menu&);
};
