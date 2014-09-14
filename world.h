#ifndef WORLD__H
#define WORLD__H

#include <string>
#include "ioManager.h"
#include "frame.h"
#include "viewport.h"

class World {
public:
  World(const std::string& name, int fact);
  ~World() { 
	  delete frame; 
	  delete flipFrame;
  }
  void update();
  void draw() const;
  const std::string& getName() const { return name; }
  void setName(const std::string& n){ name = n; }

private:
  std::string name;
  const IOManager& io;
  Frame* frame;
  Frame* flipFrame;
  int factor;
  unsigned frameWidth;
  unsigned worldWidth;
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
  World& operator=(const World&);
};

#endif
