#include "pch.h"
#include <Texture.h>
#include "Spit.h"
#include "TextureManager.h"

Spit::Spit(Point2f pos, bool toRight,const TextureManager* textureManager) : Projectile(pos,textureManager),
	m_IsMovingRight{toRight},
	//Rects
	m_DestRect{ pos.x,pos.y,8,8 },
	m_SrcRect{ 0,0,12,9 },
	m_Speed{100.f},
	//Animation
	m_FrameSpeed{ .05f }, m_AnimTime{ 0 },m_FrameWidth{12}
{
	m_pProjectileTexture = textureManager->GetTextureReference(TextureManager::TextureKey::SpitProjectileTxr);
}

void Spit::Update(float elapsedSec)
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
	if (m_IsMovingRight)
	{
		m_DestRect.left += m_Speed * elapsedSec;
	}
	else {
		m_DestRect.left -= m_Speed * elapsedSec;
	}
}

void Spit::Draw() const
{
	m_pProjectileTexture->Draw(m_DestRect,m_SrcRect);
}

const Rectf& Spit::GetHitBox() const
{
	return m_DestRect;
}

const bool Spit::CanHurtEnemy() const
{
	return true;
}