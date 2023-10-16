#pragma once
#include "Particle.h"
class AppleSplash final : public Particle
{
public:
	AppleSplash(Point2f pos, const TextureManager* textureManager);
	void Update(float elapsedSec) override;
	void Draw() const override;
};