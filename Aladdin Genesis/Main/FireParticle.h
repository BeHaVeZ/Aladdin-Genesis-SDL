#pragma once
#include "Particle.h"
class FireParticle final : public Particle
{
public:
	FireParticle(Point2f pos, const TextureManager* textureManager);
	void Update(float elapsedSec) override;
	void Draw() const override;
};