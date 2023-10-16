#pragma once
#include <Texture.h>

class TextureManager;


class Particle
{
public:
	Particle(Point2f position,const TextureManager* textureManager);
	virtual ~Particle() = default;

	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;

	const bool IsFinished() const;

protected:
	bool m_IsFinished;

	Point2f m_ParticlePos;

	const Texture* m_pParticleTexture;
	Rectf m_DestRect, m_SrcRect;
	const TextureManager* m_TextureManagerRef;

	float		m_FrameWidth;
	float		m_AnimTime;
	float		m_FrameSpeed;
};