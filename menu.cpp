#include <sstream>
#include "menu.h"

Menu::~Menu() {
  SDL_FreeSurface(surfaceOff);
  SDL_FreeSurface(surfaceOn);
}

Menu::Menu(const std::string& s) : 
  io( IOManager::getInstance() ),
  words(), 
  parser( "xmlSpec/menu.xml" ),
  position( parser.getXmlInt(s + "X"), parser.getXmlInt(s + "Y") ),
  space(),
  surfaceOff(io.loadAndSet( parser.getXmlStr("clickoffFile"), true)),
  surfaceOn(io.loadAndSet( parser.getXmlStr("clickonFile"), true)),
  clicks(),
  nextIcon(0),
  click(OFF) 
{ 
  unsigned int n = parser.getXmlInt(s + "Number");
  std::stringstream strm;
  for ( unsigned i = 0; i < n; ++i ) {
    strm << s << i;
    words.push_back( parser.getXmlStr(strm.str()) );
    strm.clear(); // clear error flags
    strm.str(std::string()); // clear contents
  }

  space[0] = parser.getXmlInt(s + "SpaceX");
  space[1] = parser.getXmlInt(s + "SpaceY");

  Frame clickoff(surfaceOff, 
              parser.getXmlInt("clickoffWidth"), 
              parser.getXmlInt("clickoffHeight"), 
              parser.getXmlInt("clickoffSrcX"), 
              parser.getXmlInt("clickoffSrcX")); 
  clicks.push_back( clickoff );

  Frame clickon(surfaceOn, 
              parser.getXmlInt("clickonWidth"), 
              parser.getXmlInt("clickonHeight"), 
              parser.getXmlInt("clickonSrcX"), 
              parser.getXmlInt("clickonSrcX")); 
  clicks.push_back( clickon );

}

void Menu::draw() const {
  int x = position[0];
  int y = position[1];
  for (unsigned i = 0; i < words.size(); ++i) {
    io.printMessageAt(words[i], x, y);
    y += space[1];
  }
  y = position[1] + nextIcon*space[1];
  // To draw the lamp, I'm using the Frame::draw method
  // that does NOT adjust for the viewport position.
  // Thus, we'll draw the lamp relative to (0, 0):
  clicks[click].draw(0, 0, position[0]-space[0], y);
}

const string& Menu::getIconClicked() const { 
  return words[nextIcon]; 
} 

