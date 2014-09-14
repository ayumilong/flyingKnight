#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "drawable.h"
#include "player.h"
#include "enemy.h"
#include "explodingSprite.h"
#include "planes.h"
#include "world.h"
#include "viewport.h"
#include "aaline.h"
#include "health.h"
#include "magic.h"
#include "sound.h"

enum worlds{
	MENUE = 41,
	SELECT = 42,
	OTHERWORLD = 43
};

enum status{
	GAME_OVER,
	GAME_WIN,
	OTHER
};

class Manager {
public:
  Manager ();
  ~Manager ();
  enum status play();
  void pause() { clock.pause(); }
  void unpause() { clock.unpause(); }
  void setNumberOfSprites(int);
  void setLevel(int);
  void updateScore(enum status);
  void stopMusic() const{
	menuSound.stopMusic();
  }
  void startMusic() const{
	  menuSound.startMusic();
  }
  int getLevel() const{
	  return level;
  }
  void setScore(int s){
	  score = s;
  }
  int getScore() const{
	  return score;
  }
  void setLife(int l){
	  life = l;
  }
  int getLife() const{
	  return life;
  }

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  int backRed;
  int backGreen;
  int backBlue;

  std::vector<Drawable*> sprites; //all the orbs
  std::vector<Planes*> planes; //all the planes that can fighting
  std::vector<World*> worlds;
  World* curWorld;
  World backWorld;
  Viewport& viewport;
  int currentSprite; //Which sprite
  enum worlds currentWorld; //Which world shows to player
  SDLSound menuSound; //play music

  bool showInfo;
  int level;
  int life;
  int score;

  Player *player;
  ExplodingSprite *explodingPlayer; 
  bool isDead;
  bool gameOver;
  Health *health;
  Magic *magic;
  std::vector<Drawable*> enemies; //All the enemies
  std::vector<Drawable*> explodingEnemies;

  void draw() const;
  void update();

  void initSprites(); //initial all the orbs
  void drawAllSprites() const; //display all the orbs
  void updateAllSprites(Uint32 ticks); //update positions of all the orbs
  void drawAllEnemies() const; //display all the enemies
  void updateAllEnemies(Uint32 ticks); //update positions of all the enemies
  void drawAllPlanes() const; //display all the planes
  void updateAllPlanes(Uint32 ticks); //update positions of all the planes
  void showInformation() const; //show some information to user

  Manager(const Manager&);
  Manager& operator=(const Manager&);
 };

class Depth{
	public:
		bool operator()(Drawable * d1, Drawable *d2){
			return d1 -> Y() < d2 -> Y();
		}
};
