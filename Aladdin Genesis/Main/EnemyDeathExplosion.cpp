#include "pch.h"
#include "EnemyDeathExplosion.h"
#include "TextureManager.h"

EnemyDeathExplosion::EnemyDeathExplosion(Point2f pos, const TextureManager* textureManager) : Particle(pos,textureManager)
{
	m_FrameSpeed = .1f;
	m_AnimTime = 0;
	m_FrameWidth = 94;

	m_pParticleTexture = textureManager->GetTextureReference(TextureManager::TextureKey::EnemyDeathExplosionTxr);
	m_DestRect = Rectf(pos.x, pos.y, 50, 50);
	m_SrcRect = Rectf(0, 0, 94, m_pParticleTexture->GetHeight());
}

void EnemyDeathExplosion::Update(float elapsedSec)
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

void EnemyDeathExplosion::Draw() const
{
	m_pParticleTexture->Draw(m_DestRect, m_SrcRect);
}