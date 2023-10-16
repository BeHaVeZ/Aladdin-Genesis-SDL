#pragma once
#include <Vector2f.h>
#include "Projectile.h"

class Knife final : public Projectile
{
public:
	Knife(Point2f pos, float angle,const TextureManager* textureManager);
	~Knife() = default;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

	const Rectf& GetHitBox() const;
	const int GetDamage() const;

	const bool CanHurtEnemy() const;

private:
	float		m_Angle;

	Rectf		m_DestRect;
	Rectf		m_SrcRect;

	Vector2f	m_Velocity;

	float		m_FrameWidth;
	float		m_AnimTime;
	float		m_FrameSpeed;

};