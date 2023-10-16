#include "pch.h"
#include "Particle.h"
#include "TextureManager.h"

Particle::Particle(Point2f pos, const TextureManager* textureManager) :
	m_ParticlePos{ pos },
	m_IsFinished{ false },
	m_FrameWidth{ 0 },
	m_AnimTime{ 0 },
	m_FrameSpeed{ 0 },
	m_DestRect{ Rectf() },
	m_SrcRect{ Rectf() },
	m_pParticleTexture{ nullptr },
	m_TextureManagerRef{textureManager}
{
}
const bool Particle::IsFinished() const
{
	return m_IsFinished;
}