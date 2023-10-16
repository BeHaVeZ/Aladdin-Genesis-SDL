#include "pch.h"
#include "HealthPickUpParticle.h"
#include "TextureManager.h"

HealthPickUpParticle::HealthPickUpParticle(Point2f pos, const TextureManager* textureManager) : Particle(pos,textureManager)
{
	m_FrameSpeed = .08f;
	m_AnimTime = 0;
	m_FrameWidth = 50;

	m_pParticleTexture = textureManager->GetTextureReference(TextureManager::TextureKey::HealthPickUpEffectTxr);
	m_DestRect = Rectf(pos.x, pos.y, 30, 30);
	m_SrcRect = Rectf(0, 0, 50, 60);
}

void HealthPickUpParticle::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.left += m_FrameWidth;

		if (m_SrcRect.left >= m_pParticleTexture->GetWidth())
		{
			m_IsFinished = true;
		}
		m_AnimTime = 0;
	}
}

void HealthPickUpParticle::Draw() const
{
	m_pParticleTexture->Draw(m_DestRect, m_SrcRect);
}
