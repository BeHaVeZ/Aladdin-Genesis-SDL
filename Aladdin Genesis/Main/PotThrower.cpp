#include "pch.h"
#include "PotThrower.h"
#include <Texture.h>
#include "Projectile.h"
#include <utils.h>
#include "Pot.h"
#include "TextureManager.h"


PotThrower::PotThrower(Point2f pos, std::vector<Projectile*>& projectiles,const SoundManager& sm, const TextureManager* textureManager) :
Enemy(pos,sm,textureManager),
m_DestRect{ Rectf(pos.x,pos.y,25,25) }, m_SrcRect{Rectf(0,0,60,50) },
m_CurrentState{ States::idle },
m_FrameSpeed{ .08f }, m_AnimTime{ 0 },
m_LevelProjectiles{ projectiles },
m_FrameWidth{ 60 },
m_DetectableRange{ 100 },
m_SpotRange(pos.x, pos.y - 100, 10, 100),
m_IsFinishedThrowing{false}
{
	m_Health = 1;
	m_CollisionPoints.push_back(Point2f(pos.x, pos.y));
	m_CollisionPoints.push_back(Point2f(pos.x, pos.y - m_DetectableRange));
	m_pEnemyTexture = textureManager->GetTextureReference(TextureManager::TextureKey::PotThrowerTxr);
}

void PotThrower::Update(float elapsedSec, const Rectf& actorShape)
{
	m_AnimTime += elapsedSec;

	if (utils::IsOverlapping(actorShape,m_SpotRange) and m_CurrentState != States::throwing)
	{
		Throw();
	}

	switch (m_CurrentState)
	{
	case PotThrower::States::throwing:
		HandleThrowingAnimation();
		break;
	}

}

void PotThrower::Draw() const
{
	m_pEnemyTexture->Draw(m_DestRect,m_SrcRect);
}

void PotThrower::Throw()
{
	m_CurrentState = States::throwing;
	m_IsFinishedThrowing = false;
}

const Rectf& PotThrower::GetHitBox() const
{
	return m_SpotRange;
}

std::vector<Point2f> PotThrower::GetEnemyDetectionPoints() const
{
	return m_CollisionPoints;
}

const bool PotThrower::IsPotThrower() const
{
	return true;
}

int PotThrower::GetCurrentHealth() const
{
	return m_Health;
}



void PotThrower::HandleThrowingAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.left += m_FrameWidth;
		if (m_SrcRect.left > m_pEnemyTexture->GetWidth())
		{
			m_SrcRect.left = 0;
			m_IsFinishedThrowing = true;
			m_CurrentState = States::idle;
		}
		else if (m_SrcRect.left >= m_FrameWidth * 7 and m_IsFinishedThrowing == false)
		{
			m_IsFinishedThrowing = true;
			m_LevelProjectiles.push_back(new Pot(Point2f(m_DestRect.left + m_DestRect.width / 2, m_DestRect.bottom ),m_TextureManagerRef));
		}
		m_AnimTime = 0;
	}
}