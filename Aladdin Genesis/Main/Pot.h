#pragma once
#include "Projectile.h"
#include <Vector2f.h>



class Pot final : public Projectile
{
public:
	Pot(Point2f spawnPos,const TextureManager* textureManager);
	~Pot() = default;

	void Update(float elapsedSec) override;
	void Draw() const override;

	const Rectf& GetHitBox() const;

	const bool CanHurtEnemy() const;

	const bool IsPotProjectile() const override;

private:

	Rectf		m_DestRect;
	Rectf		m_SrcRect;

	Vector2f	m_Velocity;

	float		m_FrameWidth;
	float		m_AnimTime;
	float		m_FrameSpeed;


};
