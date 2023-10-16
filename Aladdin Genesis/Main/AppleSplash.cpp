#include "pch.h"
#include "AppleSplash.h"
#include "TextureManager.h"

AppleSplash::AppleSplash(Point2f pos, const TextureManager* textureManager) : Particle(pos,textureManager)
{
	m_pParticleTexture = textureManager->GetTextureReference(TextureManager::TextureKey::AppleSplashTxr);
	m_DestRect = Rectf(pos.x, pos.y, 15, 15);
	m_SrcRect = Rectf(0, 0, 30, 27);

	m_FrameSpeed = .1f;
	m_AnimTime = 0;
	m_FrameWidth = 30;
}
void AppleSplash::Update(float elapsedSec)
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

void AppleSplash::Draw() const
{
	m_pParticleTexture->Draw(m_DestRect, m_SrcRect);
}