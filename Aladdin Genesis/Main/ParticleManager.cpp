#include "pch.h"
#include "ParticleManager.h"
#include <Texture.h>
#include "Particle.h"
#include "AppleSplash.h"
#include "PotExplosion.h"
#include "AppleSparkles.h"
#include "EnemyDeathExplosion.h"
#include "FireParticle.h"
#include "HealthPickUpParticle.h"
#include "TextureManager.h"

ParticleManager::ParticleManager(const TextureManager* textureManager):
	m_TextureManagerRef{textureManager}
{
}

ParticleManager::~ParticleManager()
{
	for (Particle* particle : m_Particles)
	{
		delete particle;
	}
}

void ParticleManager::PlayParticle(ParticleEffect particleToPlay, Point2f pos)
{
	switch (particleToPlay)
	{
	case ParticleEffect::appleSplash:
		m_Particles.push_back(new AppleSplash(pos,m_TextureManagerRef));
		break;
	case ParticleEffect::potExplosion:
		m_Particles.push_back(new PotExplosion(pos, m_TextureManagerRef));
		break;
	case ParticleEffect::appleSparkles:
		m_Particles.push_back(new AppleSparkles(pos, m_TextureManagerRef));
		break;
	case ParticleEffect::enemyDeathExplosion:
		m_Particles.push_back(new EnemyDeathExplosion(pos, m_TextureManagerRef));
		break;
	case ParticleEffect::fire:
		m_Particles.push_back(new FireParticle(pos, m_TextureManagerRef));
		break;
	case ParticleEffect::healthSparkles:
		m_Particles.push_back(new HealthPickUpParticle(pos, m_TextureManagerRef));
		break;
	}
}

void ParticleManager::Update(float elapsedSec)
{
	for (int i = (int)m_Particles.size() - 1; i >= 0; i--)
	{
		m_Particles[i]->Update(elapsedSec);
		if (m_Particles[i]->IsFinished() == true)
		{
			delete m_Particles[i];
			m_Particles[i] = m_Particles[m_Particles.size() - 1];
			m_Particles.pop_back();
		}
	}
}

void ParticleManager::Draw() const
{
	for (Particle* particle : m_Particles)
	{
		particle->Draw();
	}
}