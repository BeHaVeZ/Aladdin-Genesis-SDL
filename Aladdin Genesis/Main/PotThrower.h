#pragma once
#include "Enemy.h"
#include <vector>

class Projectile;

class PotThrower final : public Enemy
{
private:
	enum class States {
		idle,
		throwing
	};


public:
	PotThrower(Point2f pos, std::vector<Projectile*>& projectiles,const SoundManager& sm, const TextureManager* textureManager);
	~PotThrower() = default;

	void Update(float elapsedSec, const Rectf& actorShape) override;
	void Draw() const override;

	void Throw();

	const Rectf& GetHitBox() const override;

	std::vector<Point2f> GetEnemyDetectionPoints() const;

	const bool IsPotThrower() const override;


	int GetCurrentHealth() const override;

private:

	bool m_IsFinishedThrowing;
	States m_CurrentState;
	
	Rectf m_DestRect, m_SrcRect;
	Rectf m_SpotRange;

	void HandleThrowingAnimation();

	float		m_AnimTime;
	float		m_FrameSpeed;

	float		m_FrameWidth;

	float		m_DetectableRange;

	std::vector<Projectile*>& m_LevelProjectiles;

	std::vector<Point2f> m_CollisionPoints;

};