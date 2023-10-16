#pragma once
#include <vector>
#include "ParticleManager.h"



class LavaPool final
{
public:
	LavaPool(Point2f spawnPos, ParticleManager* particleManager);
	~LavaPool();

	std::vector<Point2f> GetPlatformPoints() const;

	void Update(float elapsedSec);

	void Explode(const Point2f& pos);

	void HasCollided(Point2f pos);

	const bool IsReadyToExplode() const;

	const Rectf& GetHitBox() const;
	const Rectf& GetLavaRect() const;

private:

	bool m_HasCollided;
	bool m_ReadyToExplode,m_HasExploded;

	float m_TimeBetweenExplosions;
	float m_Timer, m_ResetTimer,m_TimeToReset;

	const float m_Height;


	Rectf m_HitBox;
	Rectf m_LavaRect;

	Point2f m_LavaPos;
	float m_LavaPoolLength;
	std::vector<Point2f> m_LavaPoints;


	ParticleManager* m_LevelParticleManager;

	Point2f m_CollisionPos;
};