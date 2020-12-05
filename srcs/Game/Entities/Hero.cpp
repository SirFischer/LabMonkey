#include "Hero.hpp"

Hero::Hero(/* args */)
{
	mPosition = sf::Vector2f(400, 300);
	mSprite.setTexture(*ResourceManager::LoadTexture("assets/textures/Hero.png"));
	for (size_t i = 0; i < 9; i++)
		mAnimations[EntityAction::WALK_LEFT].AddFrame(sf::IntRect(64 * i, 64 * 9, 64, 64));
	mAnimations[EntityAction::WALK_LEFT].SetLength(0.1);
	for (size_t i = 0; i < 9; i++)
		mAnimations[EntityAction::WALK_RIGHT].AddFrame(sf::IntRect(64 * i, 64 * 11, 64, 64));
	mAnimations[EntityAction::WALK_RIGHT].SetLength(0.1);
	for (size_t i = 0; i < 1; i++)
		mAnimations[EntityAction::IDLE].AddFrame(sf::IntRect(64 * i, 64 * 10, 64, 64));
	mAnimations[EntityAction::IDLE].SetLength(0.08);
	for (size_t i = 0; i < 6; i++)
		mAnimations[EntityAction::JUMP].AddFrame(sf::IntRect(64 * i, 64 * 15, 64, 64));
	mAnimations[EntityAction::JUMP].SetLength(0.08);
	
}

Hero::~Hero()
{
}

void	Hero::Update()
{
	mSprite.setPosition(mPosition);
	mPosition += mVelocity;
	mVelocity.x *= 0.95;
	if (mAnimations.count(mAction))
		mSprite.setTextureRect(mAnimations[mAction].GetTextureRect());
}


void	Hero::Render(Window	*tWindow)
{
	tWindow->Draw(mSprite);
}