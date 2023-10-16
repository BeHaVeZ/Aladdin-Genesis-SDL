#pragma once
#include "Particle.h"
class EnemyDeathExplosion final : public Particle
{
public:
	EnemyDeathExplosion(Point2f pos, const TextureManager* textureManager);
	void Update(float elapsedSec) override;
	void Draw() const override;
};