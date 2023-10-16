#pragma once
#include "Projectile.h"


class Spit final : public Projectile
{
public:
	Spit(Point2f pos, bool moveRight,const TextureManager* textureManager);
	~Spit() = default;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

	const Rectf& GetHitBox() const override;


	const bool CanHurtEnemy() const;


private:

	bool
		m_IsMovingRight;

	float
		m_FrameWidth,
		m_Speed;

	Rectf m_SrcRect;
	Rectf m_DestRect;

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Animation variables
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	float
		m_AnimTime,
		m_FrameSpeed;
};
