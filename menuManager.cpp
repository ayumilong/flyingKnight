#include <cmath>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "menuManager.h"
#include "manager.h"

const int ENEMY_NUMBER = 10;

MenuManager::~MenuManager(){
	delete backWorld;
	delete gameOverMenu;
	delete gameWinMenu;
} 

MenuManager::MenuManager() :
  	env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  	screen( IOManager::getInstance().getScreen() ),
  	clock( Clock::getInstance() ),
  	backColor(),
  	menu("words"),
	levelMenu(),
	controlMenu(),
	gameOverMenu(new Menu("gameover")),
	gameWinMenu(new Menu("gamewin")),
  	backWorld(new World("menueback", Gamedata::getInstance().getXmlInt("menuebackFactor"))),
	music(true),
  	numberOfSprites(10)
{ 	
  	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
  	  throw string("Unable to initialize SDL: ");
  	}
  	backColor.r = Gamedata::getInstance().getXmlInt("backRed");
  	backColor.g = Gamedata::getInstance().getXmlInt("backGreen");
  	backColor.b = Gamedata::getInstance().getXmlInt("backBlue");
  	atexit(SDL_Quit); 
}

void MenuManager::showHighScore(){
	std::ifstream is("./highscore/highscore.dat"); //Open high score file
	std::istream_iterator<std::string> in(is); //Definition of istream_iterator with "is"
	std::istream_iterator<std::string> eos; //The default istream_iterator, indicate the end of the stream
	std::vector<std::string> vec(in, eos); //Initialize vec with istream_iterator

  	IOManager& io = IOManager::getInstance().getInstance();
  	SDL_Event event;
  	bool done = false;
  	while ( not done ) {
  	  	Uint8 *keystate = SDL_GetKeyState(NULL);
  	  	if ( SDL_PollEvent(&event) )
  	  	switch (event.type) {
  	  	  case SDL_KEYDOWN: {
  	  	    if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
  	  	      done = true;
  	  	    }
  	  	    if (keystate[SDLK_RETURN]) {
  	  	      done = true;
  	  	    }
  	  	    io.buildString(event);
  	  	  }
  	  	}
  	  	drawBackground();
  	  	const int xPos = 330;
  		unsigned int fontSize = Gamedata::getInstance().getXmlInt("fontSize"); 
		for(size_t i = 0, j = 0; i < vec.size(); i += 2, ++j){
			std::stringstream sstr;
			sstr << j + 1 << ".";
  			io.printMessageAt(sstr.str(), xPos, (j + 7) * fontSize); 
  			io.printMessageAt(vec[i], xPos + 50, (j + 7) * fontSize); 
  			io.printMessageAt(vec[i + 1], xPos + 180, (j + 7) * fontSize); 
		}
  	  	if ( !done ) {
  	    	SDL_Flip(screen);
  	  	}
  	}
}

void MenuManager::drawBackground() const {
	backWorld -> draw();
}

bool MenuManager::showGameOver(int curLevel){
	IOManager& io = IOManager::getInstance().getInstance();
  	SDL_Event event;
  	bool done = false;
  	while ( not done ) {
  	  	Uint8 *keystate = SDL_GetKeyState(NULL);
  	  	if ( SDL_PollEvent(&event) )
  	  	switch (event.type) {
  	  	  case SDL_KEYDOWN: {
  	  	    if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
  	  	      done = true;
  	  	    }
  	  	    if (keystate[SDLK_RETURN]) {
				if(gameOverMenu -> getIconClicked() == "Mani Menu"){
					done = true;
				}else if(gameOverMenu -> getIconClicked() == "Restart"){
					return true;
				}
  	  	    }
			if(keystate[SDLK_DOWN]){
				gameOverMenu -> increment();
			}
			if(keystate[SDLK_UP]){
				gameOverMenu -> decrement();
			}
  	  	    io.buildString(event);
  	  	  }
  	  	}
  	  	drawBackground();
		gameOverMenu -> draw();
		io.printMessageValueAt("GAME OVER!  Level: ", curLevel, 400, 110);
  	  	if ( !done ) {
  	    	SDL_Flip(screen);
  	  	}
  	}
	return false;
}

bool MenuManager::showGameWin(int curLevel, Manager* manager){
	IOManager& io = IOManager::getInstance().getInstance();
  	SDL_Event event;
  	bool done = false;
  	while ( not done ) {
  	  	Uint8 *keystate = SDL_GetKeyState(NULL);
  	  	if ( SDL_PollEvent(&event) )
  	  	switch (event.type) {
  	  	  case SDL_KEYDOWN: {
  	  	    if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
  	  	      done = true;
  	  	    }
  	  	    if (keystate[SDLK_RETURN]) {
				if(gameWinMenu -> getIconClicked() == "Main Menu"){
					manager -> updateScore(GAME_WIN);
					done = true;
				}else if(gameWinMenu -> getIconClicked() == "Next Level"){
					if(curLevel != 5){
						return true;
					}else{
						done = true;
					}
				}
  	  	    }
			if(keystate[SDLK_DOWN]){
				gameWinMenu -> increment();
			}
			if(keystate[SDLK_UP]){
				gameWinMenu -> decrement();
			}
  	  	    io.buildString(event);
  	  	  }
  	  	}
  	  	drawBackground();
		gameWinMenu -> draw();
		if(curLevel == 5){
			io.printMessageAt("Congratulations! You pass all level!", 300, 80);
		}
		io.printMessageValueAt("PASS LEVEL: ", levelMenu.getLevel() + 1, 380, 110);
  	  	if ( !done ) {
  	    	SDL_Flip(screen);
  	  	}
  	}
	return false;
}

void MenuManager::showSelectMenu(){
  	IOManager& io = IOManager::getInstance().getInstance();
  	SDL_Event event;
  	bool done = false;
  	while ( not done ) {
  	  	Uint8 *keystate = SDL_GetKeyState(NULL);
  	  	if ( SDL_PollEvent(&event) )
  	  	switch (event.type) {
  	  	  case SDL_KEYDOWN: {
  	  	    if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
  	  	      done = true;
  	  	    }
  	  	    if (keystate[SDLK_RETURN]) {
  	  	      done = true;
  	  	    }
			if(keystate[SDLK_RIGHT]){
				levelMenu.increment();
			}
			if(keystate[SDLK_LEFT]){
				levelMenu.decrement();
			}
  	  	    io.buildString(event);
  	  	  }
  	  	}
  	  	drawBackground();
		levelMenu.draw();
  	  	if ( !done ) {
  	    	SDL_Flip(screen);
  	  	}
  	}
}
void MenuManager::showInformation(){
  	IOManager& io = IOManager::getInstance().getInstance();
  	SDL_Event event;
  	bool done = false;
  	while ( not done ) {
  	  	Uint8 *keystate = SDL_GetKeyState(NULL);
  	  	if ( SDL_PollEvent(&event) )
  	  	switch (event.type) {
  	  	  case SDL_KEYDOWN: {
  	  	    if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
  	  	      done = true;
  	  	    }
  	  	    if (keystate[SDLK_RETURN]) {
  	  	      done = true;
  	  	    }
  	  	    io.buildString(event);
  	  	  }
  	  	}
  	  	drawBackground();
  	  	const int xPos = 330;
  		unsigned int fontSize = Gamedata::getInstance().getXmlInt("fontSize"); 
  		io.printMessageAt(std::string("Up/Down/Left/Right--Move"), xPos, 7 * fontSize); 
  		io.printMessageAt(std::string("Space--Regular Shoot"), xPos, 9 * fontSize); 
  		io.printMessageAt(std::string("Enter--Magic Shoot"), xPos, 11 * fontSize); 
  		io.printMessageAt(std::string("P--Pause"), xPos, 13 * fontSize); 
  		io.printMessageAt(std::string("Q or Esc--Quit"), xPos, 15 * fontSize); 
  	  	if ( !done ) {
  	    	SDL_Flip(screen);
  	  	}
  	}
}

void MenuManager::setGame(const Manager* manager) {
  	IOManager& io = IOManager::getInstance().getInstance();
  	SDL_Event event;
  	bool done = false;
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
					if(controlMenu.getIconClicked() == "Music"){
						controlMenu.setMusic(not controlMenu.getMusic());
						if(controlMenu.getMusic()){
							manager -> startMusic();
							music = true;
						}else{
							manager -> stopMusic();
							music = false;
						}
					}else if(controlMenu.getIconClicked() == "Easy"){
						controlMenu.setCurMode(1);
						numberOfSprites = ENEMY_NUMBER * (levelMenu.getLevel() + 1);
					}else if(controlMenu.getIconClicked() == "Normal"){
						controlMenu.setCurMode(2);
						numberOfSprites = 2 * ENEMY_NUMBER * (levelMenu.getLevel() + 1);
					}else if(controlMenu.getIconClicked() == "Hard"){
						controlMenu.setCurMode(3);
						numberOfSprites = 3 * ENEMY_NUMBER * (levelMenu.getLevel() + 1);
					}
  	  	  	  	}
				if(keystate[SDLK_DOWN]){
					controlMenu.increment();
				}
				if(keystate[SDLK_UP]){
					controlMenu.decrement();
				}
  	  	  	  	io.buildString(event);
  	  	  	}
  	  	}
  	  	drawBackground();
		controlMenu.draw();
  	  	if ( !done ) {
  	  	  	SDL_Flip(screen);
  	  	}
  	}
}

void MenuManager::play() {
  	bool keyCatch = false; // To get only 1 key per keydown
  	SDL_Event event;
  	bool done = false;
	
	Manager* manager = NULL;
  	while ( not done ) {
  	  drawBackground();
  	  menu.draw();
  	  SDL_Flip(screen);
	  if(manager == NULL){
		  manager = new Manager();
		  manager -> pause();
		  if(not music){
			  manager -> stopMusic();
		  }
	  }

  	  SDL_PollEvent(&event);
  	  if (event.type ==  SDL_QUIT) { break; }
  	  if(event.type == SDL_KEYDOWN) {
  	    switch ( event.key.keysym.sym ) {
  	      case SDLK_ESCAPE :
  	      case SDLK_q : {
  	        done = true;
  	        break;
  	      }
  	      case SDLK_RETURN : {
  	        if ( !keyCatch ) {
  	          	menu.lightOn();
  	          	if ( menu.getIconClicked() == "Start Game" ) {
  	            	// Here is where we call the play() function in Manager;
  	            	// but first, unpause the game:
					bool restart = true;
					bool nextLevel = false;
					int curLevel = levelMenu.getLevel();
					int curScore = 0;
					int curLife = 3;
					while(restart || nextLevel){
						manager -> unpause();
						manager -> setLevel(curLevel);
  	            		manager -> setNumberOfSprites( numberOfSprites );
						manager -> setScore(curScore);
						manager -> setLife(curLife);
						nextLevel = false;
						restart = false;
  	            		switch(manager -> play()){ //Game over
							case GAME_OVER:
								restart = showGameOver(manager -> getLevel());
								break;
							case GAME_WIN:
								nextLevel = showGameWin(manager -> getLevel(), manager);
								break;
							default:
								break;
						}
						delete manager;
						curLevel = manager -> getLevel();
						curScore = manager -> getScore();
						curLife = manager -> getLife() + 1; //One bonus life
						manager = NULL;
						if(restart){ //Restart from the current level
							manager = new Manager();
							manager -> pause();
							curScore = 0;
							curLife = 3;
							numberOfSprites = controlMenu.getCurMode() * ENEMY_NUMBER * (curLevel);
							--curLevel;
						}else if(nextLevel){
							manager = new Manager();
							manager -> pause();
							numberOfSprites = controlMenu.getCurMode() * ENEMY_NUMBER * (curLevel + 1);
						}else{ //Next time, start from levle 1
							numberOfSprites = controlMenu.getCurMode() * ENEMY_NUMBER;
							levelMenu.setLevel(0); //Set level to level 1
						}
					}
  	          	}
  	          	if ( menu.getIconClicked() == "Setting" ) {
					setGame(manager);
  	            	// After we get the number, we must set it to Manager:
  	          	}
  	  			if(menu.getIconClicked() == "Select Level"){
					showSelectMenu();
					numberOfSprites = controlMenu.getCurMode() * ENEMY_NUMBER * (levelMenu.getLevel() + 1); 
  	  			}
				if(menu.getIconClicked() == "High Score"){
					showHighScore();
				}
  	          	if ( menu.getIconClicked() == "Help" ) {
  	            	// Here is where we explain how to play the game"
  	  		  	showInformation();
  	          	}
  	          	if ( menu.getIconClicked() == "Exit" ) {
  	            	drawBackground();
  	            	menu.draw();
  	            	SDL_Flip(screen);
  	            	SDL_Delay(250);
  	            	done = true;
  	          	}
  	        }
  	        break;
  	      }
  	      case SDLK_DOWN   : {
  	        if ( !keyCatch ) {
  	          menu.increment();
  	        }
  	        break;
  	      }
  	      case SDLK_UP   : {
  	        if ( !keyCatch ) {
  	          menu.decrement();
  	        }
  	        break;
  	      }
  	      default          : break;
  	    }
  	    keyCatch = true;
  	  }
  	  if(event.type == SDL_KEYUP) { 
  	    keyCatch = false; 
  	    menu.lightOff();
  	  }
  	}
	delete manager;
}
