#include "pch.h"
#include "Knife.h"
#include <Texture.h>
#include <Vector2f.h>
#include "TextureManager.h"


Knife::Knife(Point2f pos,float angle,const TextureManager* textureManager): Projectile(pos,textureManager),
	m_DestRect{pos.x,pos.y,20,20},
	m_SrcRect{0,0,30,25},
	m_FrameSpeed{ .05f }, m_AnimTime{ 0 },
	m_Angle{angle},
	m_Velocity{200.f,200.f},
	m_FrameWidth{30}
{
	m_pProjectileTexture = textureManager->GetTextureReference(TextureManager::TextureKey::KnifeProjectileTxr);
}

void Knife::Update(float elapsedSec)
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
	m_DestRect.left += m_Velocity.x * cosf(m_Angle) * elapsedSec;
	m_DestRect.bottom += m_Velocity.y * sinf(m_Angle) * elapsedSec;
}

void Knife::Draw() const
{
	m_pProjectileTexture->Draw(m_DestRect, m_SrcRect);
}

const Rectf& Knife::GetHitBox() const
{
	return m_DestRect;
}

const int Knife::GetDamage() const
{
	return 2;
}

const bool Knife::CanHurtEnemy() const
{
	return false;
}
