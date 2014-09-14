#include <sstream>
#include "controlmenu.h"

ControlMenu::~ControlMenu() {
  SDL_FreeSurface(surfaceOff);
  SDL_FreeSurface(surfaceOn);
}

ControlMenu::ControlMenu() : 
  music(true),
  curMode(1),
  io( IOManager::getInstance() ),
  controls(), 
  parser( "xmlSpec/menu.xml" ),
  position( parser.getXmlInt("controlX"), parser.getXmlInt("controlY") ),
  space(),
  surfaceOff(io.loadAndSet( parser.getXmlStr("clickoffFile"), true)),
  surfaceOn(io.loadAndSet( parser.getXmlStr("clickonFile"), true)),
  clicks(),
  nextIcon(0),
  click(OFF) 
{ 
  unsigned int n = parser.getXmlInt("controlNumber");
  std::stringstream strm;
  for ( unsigned i = 0; i < n; ++i ) {
    strm << "control" << i;
    controls.push_back( parser.getXmlStr(strm.str()) );
    strm.clear(); // clear error flags
    strm.str(std::string()); // clear contents
  }

  space[0] = parser.getXmlInt("controlSpaceX");
  space[1] = parser.getXmlInt("controlSpaceY");

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

void ControlMenu::draw() const {
  int x = position[0];
  int y = position[1];
  for (unsigned i = 0; i < controls.size(); ++i) {
    io.printMessageAt(controls[i], x, y);
	if(i == 0){
		if(music){
			io.printMessageAt("On", x + 100, y);
		}else{
			io.printMessageAt("Off", x + 100, y);
		}
	}else if( i == curMode){
		io.printMessageAt("Yes", x + 100, y);
	}
    y += space[1];
  }
  y = position[1] + nextIcon*space[1];
  // To draw the lamp, I'm using the Frame::draw method
  // that does NOT adjust for the viewport position.
  // Thus, we'll draw the lamp relative to (0, 0):
  clicks[click].draw(0, 0, position[0]-space[0], y);
}

const string& ControlMenu::getIconClicked() const { 
  return controls[nextIcon]; 
} 

