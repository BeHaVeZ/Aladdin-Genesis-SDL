#pragma once

class Texture;
class HealthUI;
class TextureManager;

//----------------------------------------------------------------------------
//
//                                 Hud class
//
//----------------------------------------------------------------------------

class HudManager
{
public:
	HudManager(const TextureManager* textureManager);

	~HudManager();
	HudManager(const HudManager& other) = delete;
	HudManager& operator=(const HudManager& other) = delete;
	HudManager(HudManager&& other) = delete;
	HudManager& operator=(HudManager&& other) = delete;



	virtual void Update(float elapsedSec);
	virtual void Draw() const;


	void SetHealth(int health);

private:
	HealthUI* m_HealthUI;
};
