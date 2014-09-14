/*************************************************************************
    > File Name: bullets.h
    > Subject: 
    > Author: ayumi_Long
    > Mail: ayumilong41@gmail.com 
    > Created Time: Mon 18 Nov 2013 05:14:16 PM EST
 ************************************************************************/

#ifndef BULLETS__H
#define BULLETS__H

#include <list>
#include "bullet.h"
#include "collisionStrategy.h"

class Bullets{
	public:
		Bullets();
		~Bullets();
		void createBullet(const std::string& name, const Vector2f& pos, const Vector2f& vel, enum spritesDirection d);
		bool collisionDetect(const Drawable* d);
		void shoot(const std::string& name, const Vector2f& pos, const Vector2f& vel, enum spritesDirection d);
		bool shooting(){
			return busyList.size() > 0;
		}
		int getFreeCount(){
			return freeList.size();
		}
		int getBusyCount(){
			return busyList.size();
		}
		void update(Uint32 ticks, bool isPaused);
		void draw() const;
	private:
		std::list<Bullet*> freeList;
		std::list<Bullet*> busyList;
		CollisionStrategy* cs; 
		int timeVal;

		Bullets(const Bullets&);
		Bullets& operator=(const Bullets&);
};

#endif
