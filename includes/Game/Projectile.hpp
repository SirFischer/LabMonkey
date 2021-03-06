#pragma once
#include "Window.hpp"
#include "ResourceManager.hpp"

#include <math.h>

class Projectile
{
private:
	sf::Vector2f	mPosition;
	sf::Vector2f	mSize;
	sf::Vector2f	mVelocity;

	sf::Sprite		mSprite;

	bool			mActive = true;
	bool			mIsHit = false;

	sf::Clock		mClock;
	float			mHitLifeTime = 5.f;

	void			*mParent = NULL;

public:
	Projectile(sf::Vector2f	tPosition, float tAngle, float tForce);
	~Projectile();


	bool operator == (const Projectile& s) const { return mPosition == s.mPosition; }

	void		Update();
	void		Render(Window *tWindow);

	bool		IsActive(){return (mActive);}
	void		*GetParent(){return (mParent);}

	friend class Map;
	friend class Entity;
};

