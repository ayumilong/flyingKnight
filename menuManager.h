#include <SDL.h>
#include <vector>
#include <string>
#include "world.h"
#include "menu.h"
#include "levelmenu.h"
#include "controlmenu.h"
#include "clock.h"

class Manager;

class MenuManager {
public:
  MenuManager();
  ~MenuManager();
  void play();

private:
  bool env;
  SDL_Surface *screen;
  const Clock& clock;

  SDL_Color backColor;
  Menu menu;
  LevelMenu levelMenu;
  ControlMenu controlMenu;
  Menu *gameOverMenu;
  Menu *gameWinMenu;
  World *backWorld;

  bool music;
  int numberOfSprites;

  void drawBackground() const;
  MenuManager(const MenuManager&);
  MenuManager& operator=(const MenuManager&);
  void showInformation();
  void showHighScore();
  void showSelectMenu();
  void setGame(const Manager* manager);
  bool showGameOver(int);
  bool showGameWin(int, Manager*);
};
