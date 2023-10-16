#pragma once
#include <vector>
#include "Enemy.h"

class Knife;
class Projectile;

class RangedEnemy : public Enemy
{
public:

	enum States {
		idle,
		shooting,
		death
	};

	States m_CurrentState;

	RangedEnemy(Point2f spawnPos,std::vector<Projectile*>& projectiles,const SoundManager& sm, const TextureManager* textureManager);
	~RangedEnemy() = default;

	virtual void Update(float elapsedSec, const Rectf& actorShape) override;
	virtual void Draw() const override;
	virtual int GetCurrentHealth() const override;
	virtual bool CanBeDamaged() override;

	void Shoot(float angle);


	const Rectf& GetHitBox() const;


private:
	float		m_Timer;
	float		m_FireRate;
	Rectf		m_DestRect;
	Rectf		m_AttackZone;
	Rectf		m_SrcRect;

	std::vector<Projectile*>& m_LevelProjectiles;
};