#include "pch.h"
#include "LavaPool.h"

LavaPool::LavaPool(Point2f spawnPos, ParticleManager* particleManager) :
	m_CollisionPos{},
	m_ReadyToExplode{ true },
	m_Timer{ 0 },m_ResetTimer{0},m_TimeToReset{1.f},
	m_HasExploded{false},
	m_TimeBetweenExplosions{ .25f },
	m_HasCollided{ false },
	m_LavaPos{ spawnPos },
	m_LavaPoolLength{ 90.f },
	m_LevelParticleManager{ particleManager },
	m_Height{ 10 },
	m_HitBox{ Rectf(0,0,0,0) }
{
	m_LavaPoints.push_back(Point2f(spawnPos.x, spawnPos.y));
	m_LavaPoints.push_back(Point2f(spawnPos.x + m_LavaPoolLength, spawnPos.y));
	m_LavaRect = Rectf(spawnPos.x, spawnPos.y, m_LavaPoolLength, m_Height);
}

LavaPool::~LavaPool()
{
}

std::vector<Point2f> LavaPool::GetPlatformPoints() const
{
	return m_LavaPoints;
}

void LavaPool::Update(float elapsedSec)
{
	if (m_HasExploded == true)
	{
		m_ResetTimer += elapsedSec;

		if (m_ResetTimer >= m_TimeToReset)
		{
			m_ResetTimer = 0;
			m_HitBox = Rectf(0, 0, 0, 0);
			m_HasExploded = false;
		}
	}


	if (m_HasCollided == true)
	{
		m_Timer += elapsedSec;
		if (m_Timer >= m_TimeBetweenExplosions)
		{
			Explode(m_CollisionPos);
			m_HasCollided = false;
			m_ReadyToExplode = true;
			m_HasExploded = true;
			m_Timer = 0;
		}
	}
}

void LavaPool::Explode(const Point2f& pos)
{
	const int width{10}, height{30};

	m_HitBox = Rectf(pos.x, pos.y, (float)width, (float)height);
	m_LevelParticleManager->PlayParticle(ParticleManager::ParticleEffect::fire, pos);
}


void LavaPool::HasCollided(Point2f pos)
{
	m_CollisionPos = pos;
	m_HasCollided = true;
	m_ReadyToExplode = false;
}

const bool LavaPool::IsReadyToExplode() const
{
	return m_ReadyToExplode;
}

const Rectf& LavaPool::GetHitBox() const
{
	return m_HitBox;
}

const Rectf& LavaPool::GetLavaRect() const
{
	return m_LavaRect;
}
