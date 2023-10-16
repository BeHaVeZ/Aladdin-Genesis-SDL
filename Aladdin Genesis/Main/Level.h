#pragma once
#include <vector>
#include <Vector2f.h>

class Camel;
class Platform;
class Projectile;
class LavaPool;
class Rope;
class Player;
class FallingPlatform;
class Enemy;
class Projectile;
class SoundManager;
class ParticleManager;
class Camera;
class HudManager;
class CasketEnemy;
class HealtPickUp;
class TextureManager;
class PickUp;

//----------------------------------------------------------------------------
//
//                                 Level class
//
//----------------------------------------------------------------------------

class Level final
{
public:

	Level();
	~Level();
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& other) = delete;

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	void Update(float elapsedSec);
	void DrawBackGround(Camera* camera) const;
	void DrawForeGround() const;
	void DrawHud() const;
	void HandleCollision() const;
	bool IsOnGround(const Rectf& actorShape) const;
	bool IsColliding(const Rectf& actorShape) const;
	bool IsAgainstWall(const Rectf& actorShape) const;
	bool IsFinished() const;
	Player* GetPlayer() const;
	const Rectf GetBoundaries() const;
	SoundManager* GetLevelSoundManager() const;
	const TextureManager* GetLevelTextureManager() const;

	std::vector<Projectile*>* GetLevelProjectiles();
private:
	float							m_LevelElapsedSec;
	std::vector<Point2f>			m_Vertices;
	Rectf							m_Boundaries;
	//Level Objects
	Player* m_Player;
	Camel* m_Camel;
	Rectf m_Finish;
	bool m_IsFinished;
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	std::vector<Platform*>			m_Platforms;
	std::vector<LavaPool*>			m_Lavapools;
	std::vector<Rope*>				m_Ropes;
	std::vector<FallingPlatform*>	m_FallingPlatforms;
	std::vector<Enemy*>				m_AliveEnemies;
	std::vector<Projectile*>		m_AliveProjectiles;
	std::vector<PickUp*>			m_PickUps;
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//Level managers
	SoundManager* m_LevelSoundManager;
	ParticleManager* m_ParticleManager;
	HudManager* m_HudManager;
	const TextureManager* m_pTextureManager;
	//Level Inits
	void InitLavaPools();
	void InitRopes();
	void InitFallingPlatforms();
	void InitPlatforms();

	//Enemies
	void InitEnemies();
	void InitRangedEnemies();
	void InitPotThrowerEnemies();
	void InitGuards();
	void InitCasketEnemies();

	//Collectibles
	void InitPickUpPositions();
	void InitApples();
	void InitHealth();

	//DEBUG
	void DebugDraw() const;
};