#include <sstream>
#include "levelmenu.h"

LevelMenu::~LevelMenu() {
	for(size_t i = 0; i < levelSurface.size(); ++i){
		SDL_FreeSurface(levelSurface[i]);
	}
  SDL_FreeSurface(surfaceOff);
  SDL_FreeSurface(surfaceOn);
}

LevelMenu::LevelMenu() : 
  io( IOManager::getInstance() ),
  levelSurface(),
  levels(), 
  parser( "xmlSpec/menu.xml" ),
  position( parser.getXmlInt("levelsX"), parser.getXmlInt("levelsY") ),
  space(),
  surfaceOff(io.loadAndSet( parser.getXmlStr("clickoffFile"), true)),
  surfaceOn(io.loadAndSet( parser.getXmlStr("clickonFile"), true)),
  clicks(),
  nextIcon(0),
  click(OFF) 
{ 
  unsigned int n = parser.getXmlInt("levelsNumber");
  int width = parser.getXmlInt("levelsWidth");
  int height = parser.getXmlInt("levelsHeight");
  int srcX = parser.getXmlInt("levelsSrcX");
  int srcY = parser.getXmlInt("levelsSrcY");
  for ( unsigned i = 0; i < n; ++i ) {
	  std::stringstream sstr;
	  sstr<<"levels"<<i;
	  levelSurface.push_back(io.loadAndSet(parser.getXmlStr(sstr.str()), true));
	  levels.push_back(Frame(levelSurface[i], width, height, srcX, srcY));
  }

  space[0] = parser.getXmlInt("levelsSpaceX");
  space[1] = parser.getXmlInt("levelsSpaceY");

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

void LevelMenu::draw() const {
  int x = position[0];
  int y = position[1];
  int h = parser.getXmlInt("levelsHeight");
  int w1 = parser.getXmlInt("levelsWidth");
  for(unsigned i = 0; i < levels.size(); ++i) {
	  levels[i].draw(0, 0, x, y);
	  std::stringstream sstr;
	  sstr<<"Level "<<i + 1;
	  io.printMessageAt(sstr.str(), x + 30, y + h + 20);
	  x += space[0];
  }
  int w2 = parser.getXmlInt("clickonWidth");
  x = position[0] + nextIcon*space[0] + w1/2 - w2/2;
  // To draw the lamp, I'm using the Frame::draw method
  // that does NOT adjust for the viewport position.
  // Thus, we'll draw the lamp relative to (0, 0):
  clicks[click].draw(0, 0, x, position[1] + space[1] + 30);
}

const string& LevelMenu::getIconClicked() const { 
  return levels[nextIcon].getName(); 
} 

