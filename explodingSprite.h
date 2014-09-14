#ifndef EXPLODING__H
#define EXPLODING__H

#include <vector>
#include <list>
#include <algorithm>
#include "chunk.h"

class ExplodingSprite : public Sprite {
public:
  ExplodingSprite(const Sprite* s);
  ~ExplodingSprite();
  virtual void draw() const;
  virtual void update(Uint32 ticks, bool isPaused);
  void makeChunks(unsigned int);

  void reset(const Vector2f& pos){
	  setPosition(pos);
	  if(chunks.size() == 0){
	 	 std::copy(freeList.begin(), freeList.end(), front_inserter(chunks));
	 	 freeList.clear();
	  	 std::list<Chunk>::iterator iter = chunks.begin();
	  	 while(iter != chunks.end()){
	      	iter -> reset();
	      	iter -> setPosition(pos);
	      	++iter;
	  	 }
	  }
  }
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
private:
  std::list<Chunk> chunks; // An ExplodingSprite is a list of sprite chunks
  std::list<Chunk> freeList; // When a chunk gets out of range it goes here
  std::vector<Frame*> frames; // Each chunk has a Frame
  ExplodingSprite(const ExplodingSprite&); // Explicit disallow (Item 6)
  ExplodingSprite& operator=(const ExplodingSprite&); // (Item 6)
};

#endif
