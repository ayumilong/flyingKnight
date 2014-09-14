#include <iostream>
#include <string>
#include <sstream>
#include "sprite.h"
#include "planes.h"
#include "multisprite.h"
#include "gamedata.h"
#include "manager.h"
#include "explodingSprite.h"

Manager::~Manager() { 
  // Manager made it, so Manager needs to delete it
	for (unsigned i = 0; i < sprites.size(); ++i) {
    	delete sprites[i];
 	}
	for (unsigned i = 0; i < enemies.size(); ++i) {
    	delete enemies[i];
 	}
	for(unsigned i = 0; i < explodingEnemies.size(); ++i){
		delete explodingEnemies[i];
	}
	for (unsigned i = 0; i < planes.size(); ++i) {
    	delete planes[i];
 	}
	for(size_t i = 0; i < worlds.size(); ++i){
		delete worlds[i];
	}
	delete player;
	delete explodingPlayer;
	delete health;
	delete magic;
}

Manager::Manager() : env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  	io( IOManager::getInstance() ),
  	clock( Clock::getInstance() ),
  	screen( io.getScreen() ),
  	backRed( Gamedata::getInstance().getXmlInt("backRed") ),
  	backGreen( Gamedata::getInstance().getXmlInt("backGreen") ),
  	backBlue( Gamedata::getInstance().getXmlInt("backBlue") ),
  	sprites(), 
	planes(),
	worlds(),
	curWorld(NULL),
  	backWorld("back", Gamedata::getInstance().getXmlInt("backFactor") ),
  	viewport( Viewport::getInstance() ),
  	currentSprite(-1),
  	currentWorld(SELECT),
	menuSound(),
  	showInfo(true),
	level(1),
	life(3),
	score(0),
  	player(new Player("player", PLAYER, EAST, 1.0, 1.0, 0)),
	explodingPlayer(NULL),
	isDead(false),
	gameOver(false),
  	health(new Health()),
	magic(new Magic()),
	enemies(),
	explodingEnemies()
{
	player -> setVelocity(Vector2f(0, 0));
	player -> setPosition(Vector2f(0, 200));
  	initSprites(); //initial all the sprites
  	viewport.setObjectToTrack(player);
  	if(SDL_Init(SDL_INIT_VIDEO) != 0){
  	  throw string("Unable to initialize SDL: ");
	}
  	atexit(SDL_Quit);
}

void Manager::setLevel(int level){
	curWorld = worlds[level];
	this -> level = level + 1;
}

void Manager::setNumberOfSprites(int number) {
  	if(number > static_cast<int>(enemies.size())){
  	  number = (number - enemies.size());
  	  for (int i = 0; i < number; ++i){
  	  	enemies.push_back(new Enemy("enemysprite", FOURWAY, WEST, 1.0, 1.0, 0));
  	  }
  	}else {
  	  number = enemies.size() - number;
  	  for(int i = 0; i < number; ++i){
  	    	delete enemies.back();
  	    	enemies.pop_back();
  	  }
  	}
}

void Manager::initSprites(){ 
	std::size_t size = Gamedata::getInstance().getXmlInt("defaultSpritesNumber");
	std::size_t kind = Gamedata::getInstance().getXmlInt("kindsOfSprites");
	sprites.reserve(size * kind);
	for(unsigned int i = 0; i < size; ++i){
		sprites.push_back(new Sprite("singlesprite", SINGLE, EAST, 1.0, 1.0, 0));
	}
	for(unsigned int i = 0; i < size; ++i){
		sprites.push_back(new MultiSprite("multisprite", MULTI, EAST, 1.0, 1.0, 0));
	}
	for(unsigned int i = 0; i < size; ++i){
		sprites.push_back(new FourwaySprite("fourwaysprite", FOURWAY, EAST, 1.0, 1.0, 0));
	}
	std::size_t enemySize = Gamedata::getInstance().getXmlInt("defaultEnemiesNumber");
	enemies.reserve(enemySize);
	for(unsigned int i = 0; i < enemySize; ++i){
		enemies.push_back(new Enemy("enemysprite", FOURWAY, WEST, 1.0, 1.0, 0));
	}
	planes.push_back(new Planes("leftplane", SINGLE, NORTH, 1.0, 1.0, 0));
	planes.push_back(new Planes("upplane", SINGLE, NORTH, 1.0, 1.0, 0));
	planes.push_back(new Planes("rightplane", SINGLE, NORTH, 1.0, 1.0, 0));
	for(size_t i = 0; i < 5; ++i){
		std::stringstream sstr;
		sstr<<"level"<<i + 1;
		worlds.push_back(new World(sstr.str(), Gamedata::getInstance().getXmlInt(sstr.str() + "Factor")));
	}
	curWorld = worlds[0];
}

void Manager::showInformation() const {
  	unsigned int fontSize = Gamedata::getInstance().getXmlInt("fontSize"); 
  	const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  	io.printMessageValueAt("Level: ", level, 5, 0 * fontSize + 5);
  	io.printMessageValueAt("Enemy: ", enemies.size(), 250, 0 * fontSize + 5);
  	io.printMessageValueAt("Life: ", life, 460, 0 * fontSize + 5);
  	io.printMessageValueAt("Score: ", score, 665, 0 * fontSize + 5);
  	io.printMessageAt("Player: ", 5, 2 * fontSize);
	Draw_AALine(screen, 0, 0, 960, 0, 3, RED);
  	Draw_AALine(screen, 0, 0, 0, 1 * fontSize + 5, 3, RED);
  	Draw_AALine(screen, 960, 0, 960, 1 * fontSize + 5, 3, RED);
  	Draw_AALine(screen, 0, 1 * fontSize + 5, 960, 1 * fontSize + 5, 3, RED);
}

void Manager::drawAllSprites() const { 
	std::size_t size = sprites.size();
	for(unsigned int i = 0; i < size; ++i){
		sprites[i] -> draw();
	}
}

void Manager::updateAllSprites(Uint32 ticks){ 
	std::size_t size = sprites.size();
	for(unsigned int i = 0; i < size; ++i){
		sprites[i] -> update(ticks, clock.isPaused());
	}
  	std::sort(sprites.begin(), sprites.end(), Depth());
}
void Manager::drawAllEnemies() const { 
	static int t = 0; 
	++t;
	float posX = player -> X();
	std::size_t size = enemies.size();
	for(unsigned int i = 0; i < size; ++i){
		enemies[i] -> draw();
		if(t > 5 && abs(enemies[i] -> X() - posX) <= 500){
			static_cast<Enemy*>(enemies[i]) -> shoot(enemies[i] -> getDirection());
		}
	}
	if(t > 5){
		t = 0;
	}
	size = explodingEnemies.size();
	for(unsigned int i = 0; i< size; ++i){
		explodingEnemies[i] -> draw(); 
	}
}

void Manager::updateAllEnemies(Uint32 ticks){ 
	std::size_t size = enemies.size();
	for(unsigned int i = 0; i < size; ++i){
		enemies[i] -> update(ticks, clock.isPaused());
	}
	size = explodingEnemies.size();
	for(unsigned int i = 0; i< size; ++i){
		explodingEnemies[i] -> update(ticks, clock.isPaused());
	}
  	std::sort(enemies.begin(), enemies.end(), Depth());
}

void Manager::drawAllPlanes() const { 
	std::size_t size = planes.size();
	for(unsigned int i = 0; i < size; ++i){
		planes[i] -> draw();
	}
}

void Manager::updateAllPlanes(Uint32 ticks){ 
	std::size_t size = planes.size();
	Vector2f pos = viewport.getPosition(); 
	for(unsigned int i = 0; i < size; ++i){
		planes[i] -> update(Vector2f(pos[0] + i * 420, pos[1] + 480), ticks, clock.isPaused());
	}
}

void Manager::draw() const {
  	backWorld.draw();
  	drawAllSprites(); 
  	curWorld -> draw();
  	showInformation();
	health -> draw();
	magic -> draw();
	drawAllEnemies();
	if(not isDead){
  		player -> draw();
	}else{
		explodingPlayer -> draw();
	}
	drawAllPlanes();
	SDL_Flip(screen);
}

void Manager::update() {
	static Uint32 timeVal = 0;
	static Uint32 update = 0;
	if(not clock.isPaused()){
		++timeVal;
		++update;
	}
  	++clock;
  	Uint32 ticks = clock.getElapsedTicks();
	if(not magic -> isFull() && update%25 == 0){
		update = 0;
		magic -> updateMagic(magic -> getMagic() + 1);
	}	
	updateAllEnemies(ticks);
	std::vector<Drawable*>::iterator ptr = explodingEnemies.begin();
	while(ptr != explodingEnemies.end()){
		if(static_cast<ExplodingSprite*>(*ptr) -> chunkCount() == 0){
			delete *ptr;
			ptr = explodingEnemies.erase(ptr);
		}else{
			++ptr;
		}
	}
	std::vector<Drawable*>::iterator iter = enemies.begin();
	if(timeVal > 500){ 
		while(iter != enemies.end()){
			if(player -> collisionDetect(*iter)){
				if(health -> getHealth() != 0 && health -> getHealth() <= 40 && explodingPlayer == NULL){
					health -> updateHealth(0);
					explodingPlayer = new ExplodingSprite(static_cast<Sprite*>(player));
					isDead = true;
					--life;
					break;
				}else if(explodingPlayer == NULL){
					explodingEnemies.push_back(new ExplodingSprite(static_cast<Sprite*>(*iter)));	
					delete *iter;
					iter = enemies.erase(iter);
					health -> updateHealth(health -> getHealth() - 40);
					menuSound[1];
					continue;
				}
			}
			++iter;
		}
	}
	iter = enemies.begin();
	while(iter != enemies.end()){
		if(player -> collideWith(*iter) || planes[0] -> collideWith(*iter) || planes[1] -> collideWith(*iter) || planes[2] -> collideWith(*iter)){
			explodingEnemies.push_back(new ExplodingSprite(static_cast<Sprite*>(*iter)));	
			delete *iter;
			iter = enemies.erase(iter);
			menuSound[1];
			score += 20;
		}else{
			if(static_cast<Enemy*>(*iter) -> collideWith(player)){
				if(health -> getHealth() != 0 && health -> getHealth() <= 20 && explodingPlayer == NULL){
					health -> updateHealth(0);
					explodingPlayer = new ExplodingSprite(static_cast<Sprite*>(player));
					isDead = true;
					--life;
				}else if(explodingPlayer == NULL){
					health -> updateHealth(health -> getHealth() - 20);
					menuSound[1];
				}
			}
			++iter;
		}
	}
  	backWorld.update();
  	updateAllSprites(ticks); 
  	curWorld -> update();
  	player -> update(ticks, clock.isPaused());
	if(explodingPlayer != NULL){
		explodingPlayer -> update(ticks, clock.isPaused());
		if(explodingPlayer -> chunkCount() == 0 && isDead && not gameOver){
	   		isDead = false;
	   		player -> setPosition(explodingPlayer -> getPosition());
	   		health -> updateHealth(200);
			delete explodingPlayer;
			explodingPlayer = NULL;
			timeVal = 0;
		}	   
	}
	updateAllPlanes(ticks);
  	viewport.update(); // always update viewport last
}

void Manager::updateScore(enum status curSta){
	std::ifstream is("./highscore/highscore.dat"); //Open high score file
	std::istream_iterator<std::string> in(is); //Definition of istream_iterator with "is"
	std::istream_iterator<std::string> eos; //The default istream_iterator, indicate the end of the stream
	std::vector<std::string> vec(in, eos); //Initialize vec with istream_iterator
	bool flag = false;
	size_t i = 1;
	for(; i < vec.size(); i += 2){
		std::stringstream sstr;
		sstr << vec[i];
		int s = 0;
		sstr >> s;
		if(score >= s){
			if(score == s && i == vec.size() - 1){
				flag = false;
			}else{
				flag = true;
				break;
			}
		}
	}
	if(flag){
		IOManager& io = IOManager::getInstance().getInstance();
  		SDL_Event event;
  		bool done = false;
  		bool nameDone = false;
		bool hasWrite = false;
		string msg;
		if(curSta == GAME_OVER){
			msg = "Game over! Please input your name: ";
		}else if(curSta == GAME_WIN){
			msg = "Game win! Please input your name: ";
		}
  		io.clearString();
  		while ( not done ) {
  		  Uint8 *keystate = SDL_GetKeyState(NULL);
  		  if ( SDL_PollEvent(&event) )
  		  switch (event.type) {
  		    case SDL_KEYDOWN: {
  		      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
  		        done = true;
  		      }
  		      if (keystate[SDLK_RETURN]) {
				  if(not hasWrite){
					  hasWrite = true;
  		        	nameDone = true;
				  }else{
					  done = true;
				  }
  		      }
  		      io.buildString(event);
  		    }
  		  }
  		  io.printStringAfterMessage(msg, 150, 320);
  		  if ( nameDone ) {
  		    std::string name = io.getString();
			std::stringstream strm;
  		    strm << "Okay -- you'll see your name and score in \"High Score\"";
  		    io.printMessageAt(strm.str(), 150, 400);
			std::vector<std::string> tail;
			tail.resize(vec.size() - i + 1);
			copy(vec.begin() + i - 1, vec.end() - 2, tail.begin());
			vec.erase(vec.begin() + i - 1, vec.end());
			vec.push_back(name);
			strm.str("");
			strm.clear();
			strm << score;
			vec.push_back(strm.str());
			std::vector<std::string>::iterator iter = vec.end();
			vec.resize(vec.size() + tail.size());
			copy(tail.begin(), tail.end(), iter);
			std::ofstream os("./highscore/highscore.dat"); //Open output file
			std::ostream_iterator<std::string> out(os, "\n"); //Definition of ostream_iterator with "os"
			copy(vec.begin(), vec.end(), out); //Copy unique item of vec to "os"
			nameDone = false;
  		    SDL_Flip(screen);
  		  }
  		  if ( !done ) {
  		    SDL_Flip(screen);
  		  }
  		}
	}
}

enum status Manager::play(){
  	SDL_Event event;
  	bool done = false;
  	bool keyCatch = false;
	while(not done){
	  if(life == 0){ //Game over
		  gameOver = true;
		  update();
		  draw();
		  updateScore(GAME_OVER);
		  return GAME_OVER;
	  }
	  if(enemies.size() == 0){ //Pass a level
		  if(level == 5){
		  	  update();
		  	  draw();
			  updateScore(GAME_WIN);
		  }
		  return GAME_WIN;
	  }
  	  SDL_PollEvent(&event);
  	  Uint8 *keystate = SDL_GetKeyState(NULL);
  	  if(event.type ==  SDL_QUIT){ done = true; break; }
  	  if(event.type == SDL_KEYUP){ 
  	  	  keyCatch = false; 
  	  	  if(!keystate[SDLK_UP] && !keystate[SDLK_DOWN] && !keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT]){	
  	  	  	  player -> stop();
  	  	  }
  	  	  if(!keystate[SDLK_SPACE]){
  	  	  	  player -> stopJump();
  	  	  }
  	  }
  	  if(event.type == SDL_KEYDOWN){
  	      if(keystate[SDLK_ESCAPE] || keystate[SDLK_q]){
  	          done = true;
  	          break;
  	      }
  	      if(keystate[SDLK_p] && not keyCatch){
  	          keyCatch = true;
  	          if( clock.isPaused() ) clock.unpause();
  	          else clock.pause();
  	      }
  	      if(keystate[SDLK_F1] && not keyCatch){
  	  	      keyCatch = true;
  	  	      if(not showInfo){
  	  	  	      showInfo = true;
  	  	      }else{
  	  	  	      showInfo = false;
  	  	      } 
  	      }
  	      if(keystate[SDLK_UP]){
			  if(not isDead){
  	  	      	player -> goUp();
			  }
  	      }
  	      if(keystate[SDLK_DOWN]){
			  if(not isDead){
  	  	      	player -> goDown();
			  }
  	      }
  	      if(keystate[SDLK_LEFT]){
			  if(not isDead){
  	  	      	player -> goLeft();
			  }
  	      }
  	      if(keystate[SDLK_RIGHT]){
			  if(not isDead){
  	  	      	player -> goRight();
			  }
  	      }
  	      if(keystate[SDLK_SPACE]){
			  if(not isDead){
				 player -> shoot(player -> getDirection());
				 menuSound[0];
			  }
		  }
  	      if(keystate[SDLK_RETURN]){
			  if(magic -> getMagic() > 0){
				magic -> updateMagic(magic -> getMagic() - 1);
			  	planes[0] -> shoot("leftbullet", Vector2f(400, -400), Vector2f(65, 0));
			  	planes[1] -> shoot("upbullet", Vector2f(0, -400), Vector2f(35, 0));
			  	planes[2] -> shoot("rightbullet", Vector2f(-400, -400), Vector2f(-5, 0));
			  	menuSound[2];
			  }
		  }
	  }
  	  draw();
	  update();
  	}
	return OTHER;
}

