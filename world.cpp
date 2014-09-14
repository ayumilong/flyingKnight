#include <iostream>
#include "world.h"

World::World(const std::string& name, int fact) : 
   name(name),
  io( IOManager::getInstance() ),
  frame(new Frame(name) ),
  flipFrame(new Frame("flip" + name)),
  factor(fact),
  frameWidth( frame->getWidth() ),
  worldWidth( Gamedata::getInstance().getXmlInt("worldWidth") ),
  viewX(0.0), viewY(0.0), 
  view(Viewport::getInstance()) 
{}

void World::update() {
  viewX = static_cast<int>(view.X() / factor) % frameWidth;
  //viewY = static_cast<int>(view.Y() / factor) % frameWidth;
  viewY = view.Y();
}

void World::draw() const { 
  int n = view.X()/frameWidth;
  if(n % 2 == 0){//Odd number pictures
  	if(view.X() <= (n + 1) * frameWidth - view.getViewWidth()){
  		frame->draw(viewX, viewY, 0, 0); 
  		frame->draw(0, viewY, frameWidth - viewX, 0);
  	}else{
  	  if(view.X() <= (n + 1) * frameWidth){
  			frame->draw(viewX, viewY, 0, 0); 
  	  }else{
  			flipFrame->draw(viewX, viewY, 0, 0); 
  	  }
  		flipFrame->draw(0, viewY, frameWidth - viewX, 0);
  	}	  
  }else{//Even number pictures
  	if(view.X() <= (n + 1) * frameWidth - view.getViewWidth()){
  		flipFrame->draw(viewX, viewY, 0, 0); 
  		flipFrame->draw(0, viewY, frameWidth - viewX, 0);
  	}else{
  	  if(view.X() <= (n + 1) * frameWidth){
  			flipFrame->draw(viewX, viewY, 0, 0); 
  	  }else{
  			frame->draw(viewX, viewY, 0, 0); 
  	  }
  		frame->draw(0, viewY, frameWidth - viewX, 0);
  	}	  
  }
}

