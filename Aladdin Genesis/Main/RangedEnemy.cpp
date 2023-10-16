#include "pch.h"
#include "Projectile.h"
#include "Knife.h"
#include "RangedEnemy.h"
#include <Texture.h>
#include <utils.h>
#include "TextureManager.h"


RangedEnemy::RangedEnemy(Point2f spawnPosition, std::vector<Projectile*>& projectiles,const SoundManager& sm,const TextureManager* textureManager) : 
Enemy(spawnPosition,sm,textureManager),
//Rects
m_DestRect{ spawnPosition.x,spawnPosition.y,70,60 },
m_SrcRect{ 0,0,65,87 },
m_AttackZone{ m_DestRect.left - 150,m_DestRect.bottom - 10,250,300 },
m_FireRate{ 0.9f }, m_Timer{ 0 },
m_CurrentState{ States::idle },
m_LevelProjectiles{ projectiles }
{
	m_pEnemyTexture = textureManager->GetTextureReference(TextureManager::TextureKey::RangedEnemyTxr);
	m_Health = 2;
	m_FrameSpeed = .08f;
	m_AnimTime = 0;
	m_FrameWidth = 65;
	m_FrameHeight = m_pEnemyTexture->GetHeight();
	m_NrOfFrames = 11;
}

void RangedEnemy::Update(float elapsedSec, const Rectf& actorShape)
{
	if (m_CanBeDamaged == false)
	{
		m_DamageTimer += elapsedSec;

		if (m_DamageTimer >= m_TimeBetweenDamages)
		{
			m_CanBeDamaged = true;
			m_DamageTimer = 0;
		}
	}

	m_AnimTime += elapsedSec;
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.left += m_FrameWidth;
		if (m_SrcRect.left >= m_pEnemyTexture->GetWidth())
		{
			m_SrcRect.left = 0;
		}
		m_AnimTime = 0;
	}
	if (utils::IsOverlapping(m_AttackZone, actorShape))
	{
		m_CurrentState = States::shooting;
	}
	else {
		m_CurrentState = States::idle;
	}

	switch (m_CurrentState)
	{
	case RangedEnemy::shooting:
		m_Timer += elapsedSec;
		if (m_Timer >= m_FireRate)
		{
			Shoot(atan2f(actorShape.bottom - m_DestRect.bottom, actorShape.left - m_DestRect.left));
			m_Timer = 0;
		}
		break;
	case RangedEnemy::death:
		break;
	}
}

void RangedEnemy::Draw() const
{
	m_pEnemyTexture->Draw(m_DestRect, m_SrcRect);
}

void RangedEnemy::Shoot(float angle)
{
	m_LevelProjectiles.push_back(new Knife(Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom + m_DestRect.height / 2), angle,m_TextureManagerRef));
}

int RangedEnemy::GetCurrentHealth() const
{
	return m_Health;
}

bool RangedEnemy::CanBeDamaged()
{
	return false;
}

const Rectf& RangedEnemy::GetHitBox() const
{
	return m_DestRect;
}