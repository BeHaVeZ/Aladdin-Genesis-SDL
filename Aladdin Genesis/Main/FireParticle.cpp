#include "pch.h"
#include "FireParticle.h"
#include "TextureManager.h"

FireParticle::FireParticle(Point2f pos, const TextureManager* textureManager) : Particle(pos,textureManager)
{
	m_pParticleTexture = textureManager->GetTextureReference(TextureManager::TextureKey::FireTxr);
	m_DestRect = Rectf(pos.x, pos.y, 20, 60);
	m_SrcRect = Rectf(0,0,38,60 );
	m_FrameSpeed =.1f;
	m_AnimTime = 0;
	m_FrameWidth = 38;
}

void FireParticle::Update(float elapsedSec)
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

void FireParticle::Draw() const
{
	m_pParticleTexture->Draw(m_DestRect, m_SrcRect);
}
