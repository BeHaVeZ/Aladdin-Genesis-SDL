#pragma once
#include "Particle.h"
class PotExplosion final : public Particle
{
public:
	PotExplosion(Point2f pos, const TextureManager* textureManager);
	void Update(float elapsedSec) override;
	void Draw() const override;
};