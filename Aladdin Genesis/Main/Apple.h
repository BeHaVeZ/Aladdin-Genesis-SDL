#pragma once
#include "Projectile.h"

//----------------------------------------------------------------------------
//
//                                 Apple class
//
//----------------------------------------------------------------------------


class Apple final : public Projectile
{
public:
	//ctors
	Apple(Point2f spawnPos, bool isFlyingLeft,const TextureManager* textureManager);
	~Apple() = default;

	//basic functions
	void Update(float elapsedSec) override;
	void Draw() const override;

	//Getters
	const float GetWidth() const;
	const float GetHeight() const;

	const bool CanHurtEnemy() const;

	Rectf GetRect() const;
	const Rectf& GetHitBox() const override;

	virtual const bool IsAppleProjectile() const override;

private:

	//Physic variables
	float		m_Velocity_X;
	float		m_Velocity_Y;

	//Rects
	Rectf		m_DestRect;
	Rectf		m_SrcRect;

	//Logical Operators
	bool		m_IsFlyingLeft;

	//Rotation
	float		m_Angle;
	float		m_RotationSpeed;

};