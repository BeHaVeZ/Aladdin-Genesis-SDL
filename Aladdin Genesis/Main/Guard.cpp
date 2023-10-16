#include "pch.h"
#include "Guard.h"
#include <Texture.h>
#include <cmath>
#include "SoundManager.h"
#include "TextureManager.h"

Guard::Guard(Point2f& spawnPos, const SoundManager& sm, const TextureManager* textureManager) : Enemy(spawnPos, sm,textureManager),
m_CurrentState{ Guard::States::idle },
m_DestRect{ Rectf(spawnPos.x,spawnPos.y,70,60) }, m_SrcRect{ 0,75,100,75 },
m_Position{ Point2f(m_DestRect.left + m_DestRect.width / 2,m_DestRect.bottom + m_DestRect.height / 2) },
m_SpawnPosition{ spawnPos },
m_Velocity{ 0,0 }, m_Speed{ 40.f },
m_IsPlayerRight{ false }, m_IsWalkingOverLava{ false },
m_AttackRange{ 50 }, m_AttackHeight{ 5 }, m_AttackHeightOffset{ 5 },
m_TimeBetweenScreams{ 0 }, m_TimeBetweenTaunts{ 0 }
{
	m_Health = 3;
	m_FrameSpeed = .08f;
	m_FrameWidth = 100;
	m_FrameHeight = 75;
	m_NrOfFrames = 7;
	m_MarginBetweenFrames = 100;
	m_pEnemyTexture = textureManager->GetTextureReference(TextureManager::TextureKey::GuardTxr);
}


void Guard::Update(float elapsedSec, const Rectf& actorShape)
{

	m_TimeBetweenScreams += elapsedSec;
	m_TimeBetweenTaunts += elapsedSec;

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


	//Checkt of de speler links of rechts is
	if (playerPos.x > m_Position.x)
	{
		m_IsPlayerRight = true;
	}
	else
	{
		m_IsPlayerRight = false;
	}
	//Checken of die ver van spawnpoint is
	if (m_IsWalkingOverLava == true)
	{
		m_CurrentState = States::damagedWalking;
	}
	else if (CalculateDistance(m_Position, m_SpawnPosition) > 150)
	{
		if (m_Position.x < m_SpawnPosition.x and m_IsPlayerRight == true)
		{
			m_CurrentState = States::walking;
		}
		else if (m_Position.x > m_SpawnPosition.x and m_IsPlayerRight == false)
		{
			m_CurrentState = States::walking;
		}
		else {
			if (CalculateDistance(playerPos, m_Position) < 45) {
				m_CurrentState = States::attacking;
			}
			else {
				m_CurrentState = States::taunting;
			}
		}
	}
	//Checkt of die in zijn range is en of dat die dicht genoeg is om aan te vallen
	else if (CalculateDistance(playerPos, m_Position) < 45)
	{
		m_CurrentState = States::attacking;
	}
	else if (CalculateDistance(playerPos, m_Position) <= 100)
	{
		m_CurrentState = States::walking;
	}
	else {
		m_CurrentState = States::idle;
	}


	HandleActions();
	HandleAnimations();


	m_DestRect.left += m_Velocity.x * elapsedSec;
}

int Guard::GetCurrentHealth() const
{
	return m_Health;
}

const bool Guard::IsGuard() const
{
	return true;
}

void Guard::ReceiveDamage()
{
	if (m_CanBeDamaged == true)
	{
		m_SoundManagerRef.PlaySoundEffect(SoundManager::SoundEffects::GuardDamaged);
		--m_Health;
		m_CanBeDamaged = false;
	}
}



void Guard::Draw() const
{
	if (m_IsPlayerRight == true)
	{
		glPushMatrix();
		{
			glTranslatef(m_DestRect.left, m_DestRect.bottom, 0);
			glScalef(-1, 1, 1);
			glTranslatef(-m_DestRect.width - m_DestRect.width / 2, 0, 0);
		}
		m_pEnemyTexture->Draw(Rectf(0, 0, m_DestRect.width, m_DestRect.height), m_SrcRect);
		glPopMatrix();
	}
	else {
		m_pEnemyTexture->Draw(m_DestRect, m_SrcRect);
	}
}

void Guard::IsWalkingOverLava(bool flag)
{
	m_IsWalkingOverLava = flag;
}

const Rectf& Guard::GetHitBox() const
{
	return Rectf(m_DestRect.left + m_DestRect.width - m_DestRect.width / 2, m_DestRect.bottom, m_DestRect.width * .5f, m_DestRect.height);
}

const Rectf& Guard::GetAttackZone() const
{
	return m_AttackZone;
}

void Guard::HandleActions()
{
	m_AttackZone = Rectf(0, 0, 0, 0);
	switch (m_CurrentState)
	{
	case Guard::States::idle:
		m_Velocity.x = 0;
		break;
	case Guard::States::walking:
		if (m_IsPlayerRight == true)
		{
			m_Velocity.x = m_Speed;
		}
		else {
			m_Velocity.x = -m_Speed;
		}
		break;
	case Guard::States::damaged:
		break;
	case Guard::States::attacking:
		m_Velocity.x = 0;
		break;
	case Guard::States::damagedWalking:
		if (m_IsPlayerRight == true)
		{
			m_Velocity.x = m_Speed;
		}
		else {
			m_Velocity.x = -m_Speed;
		}
		break;
	case Guard::States::taunting:
		m_Velocity.x = 0;
		break;
	}
}

void Guard::HandleAnimations()
{
	switch (m_CurrentState)
	{
	case Guard::States::idle:
		HandleIdleAnimation();
		break;
	case Guard::States::walking:
		HandleWalkingAnimation();
		break;
	case Guard::States::damaged:
		HandleDamagedAnimation();
		break;
	case Guard::States::attacking:
		HandleAttackingAnimation();
		break;
	case Guard::States::damagedWalking:
		HandleDamagedWalkingAnimation();
		break;
	case Guard::States::taunting:
		HandleTauntAnimation();
		break;
	}
}

float Guard::CalculateDistance(const Point2f& p1, const Point2f& p2)
{
	float distance = std::abs(p1.x - p2.x);
	return distance;
}

void Guard::HandleIdleAnimation()
{
	//checks the frame from the previous one
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = m_FrameHeight;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= m_MarginBetweenFrames * 7)
		{
			m_SrcRect.left = 0;
		}
		m_AnimTime = 0;
	}
}

void Guard::HandleWalkingAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = m_FrameHeight * 2;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= m_MarginBetweenFrames * 7)
		{
			m_SrcRect.left = 0;
		}
		m_AnimTime = 0;
	}
}

void Guard::HandleDamagedAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = m_FrameHeight * 5;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= m_MarginBetweenFrames * 7)
		{
			m_SrcRect.left = m_MarginBetweenFrames * 2;
		}
		m_AnimTime = 0;
	}
}

void Guard::HandleAttackingAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = m_FrameHeight * 4;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= m_MarginBetweenFrames * 7)
		{
			m_SrcRect.left = m_MarginBetweenFrames * 2;
			if (m_IsPlayerRight == true)
			{
				m_AttackZone = Rectf(m_DestRect.left, m_DestRect.bottom + m_AttackHeightOffset, m_AttackRange * 2, m_AttackHeight);
			}
			else
			{
				m_AttackZone = Rectf(m_DestRect.left, m_DestRect.bottom + m_AttackHeightOffset, m_AttackRange * 2, m_AttackHeight);
			}
		}
		m_AnimTime = 0;
	}
}

void Guard::HandleDamagedWalkingAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = m_FrameHeight * 6;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= m_MarginBetweenFrames * 7)
		{
			m_SrcRect.left = m_MarginBetweenFrames * 2;
			if (m_TimeBetweenScreams > 1)
			{
				m_SoundManagerRef.PlaySoundEffect(SoundManager::SoundEffects::GuardsPants);
				m_TimeBetweenScreams = 0;
			}

		}
		m_AnimTime = 0;
	}
}

void Guard::HandleTauntAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = m_FrameHeight * 3;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= m_MarginBetweenFrames * 7)
		{
			if (m_TimeBetweenTaunts > 2)
			{
				m_SoundManagerRef.PlaySoundEffect(SoundManager::SoundEffects::GuardHit1);
				m_TimeBetweenTaunts = 0;
			}
			m_SrcRect.left = m_MarginBetweenFrames * 2;
		}
		m_AnimTime = 0;
	}
}
