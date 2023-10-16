#pragma once
#include "Particle.h"
class AppleSparkles final : public Particle
{
public:
	AppleSparkles(Point2f pos, const TextureManager* textureManager);
	void Update(float elapsedSec) override;
	void Draw() const override;
};