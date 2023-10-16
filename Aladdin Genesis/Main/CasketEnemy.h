#pragma once
#include "Enemy.h"


class CasketEnemy : public Enemy
{
public:

	enum class States {
		idle,
		looking,
		attacking
	};

	CasketEnemy(Point2f& pos,const SoundManager& sm ,const TextureManager* textureManager);
	~CasketEnemy() = default;

	virtual void Update(float elapsedSec,const Rectf& actorShape) override;
	virtual void Draw() const override;
	virtual int GetCurrentHealth() const;
	virtual const bool IsCasketEnemy() const;
	const Rectf& GetHitBox() const;
	const Rectf& GetAttackZone() const;

private:

	bool m_IsAttacking;
	bool m_IsPlayerRight;

	States m_CurrentState;
	
	Rectf m_DestRect, m_SrcRect;
	Rectf m_AttackZone;

	float m_AttackRange;
	float m_AttackHeight;
	float m_AttackHeightOffset;



	void HandleAnimations();
	void HandleIdleAnim();
	void HandleLookingAnim();
	void HandleAttackingAnim();

	float CalculateDistance(const Point2f& p1, const Point2f& p2);
	float CalculateDistanceOverY(const float p1, const float p2);

};