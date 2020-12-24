#include "Entity.hpp"

Entity::Entity(/* args */)
{
}

Entity::~Entity()
{
}

void				Entity::ApplyAnimation()
{
	if (mAnimations.count(mAction))
		mSprite.setTextureRect(mAnimations[mAction].GetTextureRect());
	mAction = EntityAction::IDLE;
}


void				Entity::Attack(std::list<Entity *> tEntities, std::list<ParticleEffect> *tParticleEffects)
{
	if (mAttackClock.getElapsedTime().asSeconds() < mAttackCooldown)
		return ;
	mAttackClock.restart();
	(void)tParticleEffects;
	if (mDirection == Direction::LEFT)
		mAction = EntityAction::ATTACK_LEFT;
	else
		mAction = EntityAction::ATTACK_RIGHT;
	if (mAnimations[mAction].IsDone())
	{
		mAnimations[mAction].ResetAnimation();
		for (auto &entity : tEntities)
		{
			//CHECK Y AXIS
			if (((entity->GetPosition().y < mPosition.y + mSize.y && entity->GetPosition().y + entity->GetSize().y > mPosition.y)) &&
				((mDirection == Direction::LEFT && (entity->GetPosition().x + entity->GetSize().x > mPosition.x - (mSize.x / 2.f) && entity->GetPosition().x < mPosition.x + (mSize.x / 2.f))) ||
				(mDirection == Direction::RIGHT && (entity->GetPosition().x + entity->GetSize().x > mPosition.x + (mSize.x / 2.f) && entity->GetPosition().x < mPosition.x + (1.5 * mSize.x)))))
			{
				entity->mHealth -= mAttackDamage;
				entity->mDeathClock.restart();
				entity->mAttackClock.restart();
				sf::Vector2f	particlePos = entity->GetPosition() + (mSize / 2.f);
				if (entity->GetPosition().x + (entity->mSize.x / 2.f) < mPosition.x + (mSize.x / 2.f))
				{
					entity->mVelocity = sf::Vector2f(-6, -2);
					particlePos.x -= 10;
				}
				else
				{
					entity->mVelocity = sf::Vector2f(6, -2);
					particlePos.x += 10;
				}
				float angle = std::atan2(mPosition.y - entity->GetPosition().y, mPosition.x - entity->GetPosition().x) + M_PI;
				ParticleEffect effect(particlePos, 3.f, 0.8f, 300, angle, M_PI / 3.5f);
				effect.SetParticleColor(sf::Color::Red);
				tParticleEffects->push_back(effect);
			}
		}
	}	
}


void				Entity::Attack(Entity *tEntity, std::list<ParticleEffect> *tParticleEffects)
{
	(void)tParticleEffects;
	if (mAttackClock.getElapsedTime().asSeconds() < mAttackCooldown)
		return ;
	mAttackClock.restart();
	if (mDirection == Direction::LEFT)
		mAction = EntityAction::ATTACK_LEFT;
	else
		mAction = EntityAction::ATTACK_RIGHT;
	if (mAnimations[mAction].IsDone())
	{
		mAnimations[mAction].ResetAnimation();
	
		if (tEntity->mSprite.getGlobalBounds().intersects(mSprite.getGlobalBounds()))
		{
			tEntity->mHealth -= mAttackDamage;
			tEntity->mDeathClock.restart();
			sf::Vector2f	particlePos = tEntity->GetPosition() + (mSize / 2.f);
			if (tEntity->GetPosition().x + (tEntity->mSize.x / 2.f) < mPosition.x + (mSize.x / 2.f))
			{
				tEntity->mVelocity += sf::Vector2f(-5, -1);
				particlePos.x -= 10;
			}
			else
			{
				tEntity->mVelocity += sf::Vector2f(5, -1);
				particlePos.x += 10;

			}
			float angle = std::atan2(mPosition.y - tEntity->GetPosition().y, mPosition.x - tEntity->GetPosition().x) + M_PI;
			ParticleEffect effect(particlePos, 3.f, 0.8f, 300, angle, M_PI / 3.5f);
			effect.SetParticleColor(sf::Color::Red);
			tParticleEffects->push_back(effect);
		}
	}	
}

void	Entity::Shoot(std::list<Projectile> *tProjectiles)
{
	if (mAttackClock.getElapsedTime().asSeconds() < mAttackCooldown)
		return ;
	mAttackClock.restart();
	if (mDirection == Direction::LEFT)
		mAction = EntityAction::SHOOT_LEFT;
	else
		mAction = EntityAction::SHOOT_RIGHT;
	
	if (mAnimations[mAction].IsDone())
	{
		mAnimations[mAction].ResetAnimation();
		Projectile projectile(mPosition + sf::Vector2f(20, 30), (mDirection == Direction::LEFT) ? M_PI : 0, 15.f);
		tProjectiles->push_back(projectile);
		std::cout << "added projectile to list: " << tProjectiles->size() << std::endl;
	}
}

void				Entity::Reset()
{
	mHealth = 30;
	mIsAlive = true;
	mVelocity = sf::Vector2f(0, 0);
}


void				Entity::MoveLeft()
{

	if (mOnGround)
		mVelocity.x -= mSpeed;
	else
		mVelocity.x -= mSpeed / 4.f;
	mAction = EntityAction::WALK_LEFT;
	mDirection = Direction::LEFT;
}

void				Entity::MoveRight()
{
	if (mOnGround)
		mVelocity.x += mSpeed;
	else
		mVelocity.x += mSpeed / 4.f;
	mAction = EntityAction::WALK_RIGHT;
	mDirection = Direction::RIGHT;
}

void				Entity::RunLeft()
{
	mVelocity.x -= mRunSpeed;
	mAction = EntityAction::WALK_LEFT;
	mDirection = Direction::LEFT;
}

void				Entity::RunRight()
{
	mVelocity.x += mRunSpeed;
	
	mAction = EntityAction::WALK_RIGHT;
	mDirection = Direction::RIGHT;
}

void				Entity::Jump()
{
	if (mOnGround)
	{
		if (mVelocity.y > 0 && (mCollisionDirection == Entity::CollisionDirection::LEFT || mCollisionDirection == Entity::CollisionDirection::RIGHT))
		{
			if (mCollisionDirection == Entity::CollisionDirection::LEFT)
				mVelocity.x += 5.f;
			else
				mVelocity.x -= 5.f;
			mVelocity.y -= mJumpForce / 1.5f;
		}
		else
			mVelocity.y -= mJumpForce;
	}
	
}

void				Entity::HandleCollisions(std::list<Entity *> tEntities)
{
	sf::Vector2f	position = GetPosition() + mVelocity;
	sf::Vector2f	prevposition = GetPosition();
	for (auto &entity : tEntities)
	{
		if (entity == this)
			continue ;
		if (std::abs(position.x - entity->mPosition.x) < entity->mSize.x * 3.f && std::abs(position.y - entity->mPosition.y) < entity->mSize.y * 3.f)
		{
			
			int mx = entity->mPosition.x;
			int my = entity->mPosition.y;
			//AABB collision?
			if ((position.x < (mx + entity->mSize.x) && (position.x + mSize.x) > mx) && (position.y < (my + entity->mSize.y) && (position.y + mSize.y) > my))
			{
				float middleX =  (position.x + (mSize.x / 2.0)) - (mx + (entity->mSize.x / 2.0));
				float middleY = (position.y + (mSize.y / 2.0)) - (my + (entity->mSize.y / 2.0));
				float angle = std::atan2(middleY, middleX) + M_PI;
				angle = ((angle / 3.1416) * 180.0);
				if ((angle > 45 && angle < 135) && mVelocity.y > 0)
				{
					mVelocity.y = 0;
					mOnGround = true;
					mCollisionDirection = Entity::CollisionDirection::BOTTOM;
					mPosition = sf::Vector2f(prevposition.x, my - mSize.y);
					
				} else 
				if ((angle < 45 || angle > 315) && mVelocity.x > 0)
				{
					mVelocity.x = 0;
					mPosition = sf::Vector2f(mx - mSize.x, prevposition.y);
					mCollisionDirection = Entity::CollisionDirection::RIGHT;
					mIsBlocked = true;
				} else if ((angle > 135 && angle < 225) && mVelocity.x < 0)
				{
					mVelocity.x = 0;
					mCollisionDirection = Entity::CollisionDirection::LEFT;
					mIsBlocked = true;
					mPosition = sf::Vector2f(mx + entity->mSize.x, prevposition.y);
				} else if ((angle > 235  && angle < 305) && mVelocity.y < 0)
				{
					mVelocity.y = 0;
					mCollisionDirection = Entity::CollisionDirection::TOP;
					mPosition = sf::Vector2f(prevposition.x, my + entity->mSize.y);
				}
				position = GetPosition() + mVelocity;
				prevposition = GetPosition();
			}
		}
	}
}