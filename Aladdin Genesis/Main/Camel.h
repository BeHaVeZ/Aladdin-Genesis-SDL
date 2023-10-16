#pragma once
#include <vector>
//class forwards
class Texture;
class Spit;
class Projectile;
class TextureManager;

//----------------------------------------------------------------------------
//
//                                 Camel class
//
//----------------------------------------------------------------------------

class Camel final
{
public:
	//Constructor
	Camel(const Point2f& position,std::vector<Projectile*>& projectiles,const TextureManager& textureManager);
	//Destructor
	~Camel() = default;

	//Basic functions
	void Draw() const;
	void Update(float elapsedSec);
	void Shoot();

	//Camel States
	enum class CamelStates {
		idle,
		shooting
	};
	CamelStates m_CurrentCamelState;
	//Logical Operators
	bool m_HasShot;

	//Getters
	std::vector<Point2f> GetCamelCollisionPoints() const;

private:
	//Pointer to texture on heap on init
	const TextureManager& m_TextureManager;
	const Texture* m_CamelTexture;

	Rectf m_DestRect;
	Rectf m_SrcRect;

	std::vector<Point2f> m_CamelCollisionPoints;

	std::vector<Projectile*>& m_LevelProjectiles;

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Animation variables
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	float
		m_NrFrames,
		m_FrameWidth,
		m_AnimTime,
		m_FrameSpeed;

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Animation functions
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	void 
		HandleIdleAnimation(),
		HandleShootingAnimation();


};