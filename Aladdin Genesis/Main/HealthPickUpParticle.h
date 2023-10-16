#pragma once
#include "Particle.h"
class HealthPickUpParticle final : public Particle
{
public:
	HealthPickUpParticle(Point2f pos, const TextureManager* textureManager);
	void Update(float elapsedSec) override;
	void Draw() const override;
};