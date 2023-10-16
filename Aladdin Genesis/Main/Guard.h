#pragma once
#include "Enemy.h"
#include <Vector2f.h>


//----------------------------------------------------------------------------
//
//                                Guard class
//
//----------------------------------------------------------------------------

class Guard final : public Enemy
{
public:
	//Possible States of the guard
	enum class States {
		idle,
		walking,
		damaged,
		attacking,
		damagedWalking,
		taunting
	};

	States m_CurrentState;

	Guard(Point2f& spawnPos,const SoundManager& sm, const TextureManager* textureManager);
	~Guard() = default;



	virtual void Update(float elapsedSec, const Rectf& actorShape) override;
	virtual int GetCurrentHealth() const;
	virtual const bool IsGuard() const override;
	virtual void ReceiveDamage() override;
	virtual void Draw() const override;



	void IsWalkingOverLava(bool flag);

	const Rectf& GetHitBox() const;
	const Rectf& GetAttackZone() const;

private:
	bool m_IsPlayerRight;
	bool m_IsWalkingOverLava;

	Rectf m_DestRect;
	Rectf m_SrcRect;
	Rectf m_AttackZone;
	Point2f m_Position;
	Point2f m_SpawnPosition;

	Vector2f m_Velocity;
	float m_Speed;
	float m_AttackRange;
	float m_AttackHeight;
	float m_AttackHeightOffset;

	float m_TimeBetweenScreams;
	float m_TimeBetweenTaunts;


	void HandleActions();
	void HandleAnimations();

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Calculation functions
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	float CalculateDistance(const Point2f& p1, const Point2f& p2);

	


	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Animation functions
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	void HandleIdleAnimation();
	void HandleWalkingAnimation();
	void HandleDamagedAnimation();
	void HandleAttackingAnimation();
	void HandleDamagedWalkingAnimation();
	void HandleTauntAnimation();

};