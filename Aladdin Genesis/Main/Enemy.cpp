#include "pch.h"
#include "Enemy.h"
#include "SoundManager.h"
#include "TextureManager.h"

Enemy::Enemy(Point2f& startPos,const SoundManager& sm,const TextureManager* textureManger) : 
m_SoundManagerRef(sm),
m_Position(startPos),
m_Health{ 0 },
m_AnimTime{ 0 },
m_FrameHeight{ 0 },
m_FrameSpeed{ 0 },
m_FrameWidth{ 0 },
m_NrOfFrames{ 0 },
m_MarginBetweenFrames{ 0 },
m_CanBeDamaged{ true },
m_TimeBetweenDamages{ 1.f },
m_DamageTimer{0},
m_TextureManagerRef{textureManger},
m_pEnemyTexture{nullptr}
{
}

const bool Enemy::IsPotThrower() const
{
	return false;
}

const bool Enemy::IsGuard() const
{
	return false;
}

const bool Enemy::IsCasketEnemy() const
{
	return false;
}

bool Enemy::CanBeDamaged()
{
	return m_CanBeDamaged;
}

void Enemy::ReceiveDamage()
{
	if (m_CanBeDamaged == true)
	{
		m_SoundManagerRef.PlaySoundEffect(SoundManager::SoundEffects::EnemyDamaged);
		--m_Health;
		m_CanBeDamaged = false;
	}
}
