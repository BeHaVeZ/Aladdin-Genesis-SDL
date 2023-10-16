#pragma once
#include <vector>

class Particle;
class Texture;
class TextureManager;

class ParticleManager final
{
public:
	enum class ParticleEffect {
		appleSplash = 1,
		potExplosion,
		appleSparkles,
		enemyDeathExplosion,
		fire,
		healthSparkles
	};

	ParticleManager(const TextureManager* textureManager);

	~ParticleManager();
	ParticleManager(const ParticleManager& other) = delete;
	ParticleManager& operator=(const ParticleManager& other) = delete;
	ParticleManager(ParticleManager&& other) = delete;
	ParticleManager& operator=(ParticleManager&& other) = delete;

	void PlayParticle(ParticleEffect particleToPlay, Point2f pos);


	void Update(float elapsedSec);
	void Draw() const;
private:
	std::vector<Particle*> m_Particles;
	const TextureManager* m_TextureManagerRef;
};