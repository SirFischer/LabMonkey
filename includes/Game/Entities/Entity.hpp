#pragma once
#include "Window.hpp"
#include "Animation.hpp"
#include "EntityAction.hpp"

#include <map>

class Entity
{
protected:
	sf::Vector2f				mSize = sf::Vector2f(32, 48);
	sf::Vector2f				mPosition = sf::Vector2f(0, 0);
	sf::Vector2f				mVelocity = sf::Vector2f(0, 0);
	float						mSpeed = 0.35f;
	float						mJumpForce = 6.f;
	bool						mOnGround = false;

	sf::Sprite					mSprite;

	EntityAction				mAction = EntityAction::IDLE;

	std::map<EntityAction, Animation>	mAnimations = std::map<EntityAction, Animation>();

public:
	Entity(/* args */);
	~Entity();

	virtual void				Update() = 0;
	virtual void				Render(Window *tWindow) = 0;

	virtual void				Attack();
	void						MoveLeft();
	void						MoveRight();
	void						Jump();

	sf::Vector2f				GetPosition(){return (mPosition);}
	//sf::Vector2f				GetSize(){return (sf::Vector2f(mSprite.getGlobalBounds().width, mSprite.getGlobalBounds().height));}
	sf::Vector2f				GetSize(){return (mSize);}
	friend class				Map;
};

