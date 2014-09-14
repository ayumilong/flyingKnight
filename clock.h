#ifndef CLOCK_H
#define CLOCK_H

#include <SDL.h>
#include <string>
#include <iostream>
#include <fstream>

class Manager;
class Player;

class Clock {
public:
  static Clock& getInstance();
  unsigned getTicks() const;
  bool isPaused() const  { return paused;  }

private:
  friend class Manager;
  friend class Player;
  unsigned getElapsedTicks();
  Clock& operator++();
  Clock  operator++(int);
  bool isStarted() const { return started; }
  unsigned getFrames() const  { return frames;  }
  unsigned getSeconds() const { return getTicks()/1000;  }
  int getFps() const;

  void start();
  void pause();
  void unpause();
  void debug();

  bool started;
  bool paused;
  unsigned frames;
  unsigned timeAtStart;
  unsigned timeAtPause;
  unsigned currTicks;
  unsigned prevTicks;
  unsigned ticks;

  Clock();
  Clock(const Clock&);
  Clock&operator=(const Clock&);
};

#endif
