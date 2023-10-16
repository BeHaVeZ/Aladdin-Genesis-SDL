#include "pch.h"
#include "Pot.h"
#include <Texture.h>
#include "TextureManager.h"

Pot::Pot(Point2f spawnPos,const TextureManager* textureManager) : 
	Projectile(spawnPos,textureManager),
	m_DestRect{spawnPos.x,spawnPos.y,20,20},
	m_SrcRect{0,0,38,30},
	m_FrameSpeed{ .08f }, m_AnimTime{ 0 }, m_FrameWidth{ 38 },
	m_Velocity{2.f,-50.f}
{
	m_pProjectileTexture = textureManager->GetTextureReference(TextureManager::TextureKey::PotProjectileTxr);
}


void Pot::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.left += m_FrameWidth;

		if (m_SrcRect.left >= m_pProjectileTexture->GetWidth())
		{
			m_SrcRect.left = 0;
		}
		m_AnimTime = 0;
	}
	m_DestRect.left += m_Velocity.x * elapsedSec;
	m_DestRect.bottom += m_Velocity.y * elapsedSec;
}

void Pot::Draw() const
{
	m_pProjectileTexture->Draw(m_DestRect, m_SrcRect);
}

const Rectf& Pot::GetHitBox() const
{
	return m_DestRect;
}

const bool Pot::CanHurtEnemy() const
{
	return false;
}

const bool Pot::IsPotProjectile() const
{
	return true;
}
