#include "pch.h"
#include "CasketEnemy.h"
#include <Texture.h>
#include "TextureManager.h"



CasketEnemy::CasketEnemy(Point2f& spawnPos, const SoundManager& sm, const TextureManager* textureManager) : Enemy(spawnPos, sm,textureManager),
m_CurrentState{ States::idle },
m_IsAttacking{ false }, m_IsPlayerRight{ false },
m_DestRect{ Rectf(spawnPos.x,spawnPos.y,40,50) }, m_SrcRect{ 0,70,60,70 },
m_AttackRange{ 20 }, m_AttackHeight{ 5 }, m_AttackHeightOffset{ 15 }
{
	m_Health = 2;
	m_FrameSpeed = .08f;
	m_FrameWidth = 60;
	m_FrameHeight = 70;
	m_NrOfFrames = 7;
	m_MarginBetweenFrames = 60;
	m_pEnemyTexture = textureManager->GetTextureReference(TextureManager::TextureKey::CasketEnemyTxr);
}

void CasketEnemy::Update(float elapsedSec, const Rectf& actorShape)
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

	Point2f playerPos{ Point2f(actorShape.left + actorShape.width / 2,actorShape.bottom + actorShape.height / 2) };
	m_Position = Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom + m_DestRect.height / 2);
	m_AnimTime += elapsedSec;


	if (playerPos.x < m_Position.x)
	{
		m_IsPlayerRight = true;
	}
	else
	{
		m_IsPlayerRight = false;
	}


	if (CalculateDistance(playerPos, m_Position) <= 30 and CalculateDistanceOverY(playerPos.y,m_Position.y	) <= 20)
	{
		m_CurrentState = States::attacking;
	}
	else if (CalculateDistance(playerPos,m_Position) <= 100)
	{
		m_CurrentState = States::looking;
	}
	else
	{
		m_CurrentState = States::idle;
	}

	HandleAnimations();
}

void CasketEnemy::Draw() const
{
	if (m_IsPlayerRight == true)
	{
		glPushMatrix();
		{
			glTranslatef(m_DestRect.left, m_DestRect.bottom, 0);
			glScalef(-1, 1, 1);
			glTranslatef(-m_DestRect.width + m_DestRect.width / 2, 0, 0);
		}
		m_pEnemyTexture->Draw(Rectf(0, 0, m_DestRect.width, m_DestRect.height), m_SrcRect);
		glPopMatrix();
	}
	else {
		m_pEnemyTexture->Draw(m_DestRect, m_SrcRect);
	}
}

int CasketEnemy::GetCurrentHealth() const
{
	return m_Health;
}

const bool CasketEnemy::IsCasketEnemy() const
{
	return true;
}

const Rectf& CasketEnemy::GetHitBox() const
{
	return m_DestRect;
}

const Rectf& CasketEnemy::GetAttackZone() const
{
	return m_AttackZone;
}

void CasketEnemy::HandleAnimations()
{
	switch (m_CurrentState)
	{
	case CasketEnemy::States::idle:
		HandleIdleAnim();
		break;
	case CasketEnemy::States::looking:
		HandleLookingAnim();
		break;
	case CasketEnemy::States::attacking:
		HandleAttackingAnim();
		break;
	}
}

void CasketEnemy::HandleIdleAnim()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = m_FrameHeight;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= m_MarginBetweenFrames * 1)
		{
			m_SrcRect.left = 0;
			m_AttackZone = Rectf();
		}
		m_AnimTime = 0;
	}
}

void CasketEnemy::HandleLookingAnim()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = m_FrameHeight;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= m_MarginBetweenFrames * 7)
		{
			m_SrcRect.left = m_MarginBetweenFrames * 4;
			m_AttackZone = Rectf();
		}
		m_AnimTime = 0;
	}
}

void CasketEnemy::HandleAttackingAnim()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = m_FrameHeight * 2;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= m_MarginBetweenFrames * 7)
		{
			m_SrcRect.left = 0;
			if (m_IsPlayerRight == true)
			{
				m_AttackZone = Rectf(m_DestRect.left - m_AttackRange, m_DestRect.bottom + m_AttackHeightOffset, m_AttackRange, m_AttackHeight);
			}
			else
			{
				m_AttackZone = Rectf(m_DestRect.left + m_DestRect.width - m_AttackHeightOffset, m_DestRect.bottom + m_AttackHeightOffset, m_AttackRange, m_AttackHeight);
			}
		}
		m_AnimTime = 0;
	}
}

float CasketEnemy::CalculateDistance(const Point2f& p1, const Point2f& p2)
{
	float distance = std::fabs(p1.x - p2.x);
	return distance;
}

float CasketEnemy::CalculateDistanceOverY(const float p1, const float p2)
{
	return std::fabs(p1 - p2);;
}