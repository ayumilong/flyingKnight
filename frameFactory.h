#include <string>
#include <vector>
#include <map>
#include "frame.h"
#include "gamedata.h"
#include "drawable.h"

class FrameFactory {
public:
  static FrameFactory& getInstance();
  ~FrameFactory();
  Frame* getFrame(const std::string&);
  std::vector<const Frame*> getFrames(const std::string&, enum spritesType);

private:
  const Gamedata& gdata;
  std::map<std::string, Frame*> frames;
  std::map<std::string, std::vector<const Frame*> > multiFrames;

  FrameFactory() : 
    gdata( Gamedata::getInstance() ), 
    frames(),
    multiFrames()
  {}
  FrameFactory(const FrameFactory&);
  FrameFactory& operator=(const FrameFactory&);
};
