#include "pch.h"
#include "HudManager.h"
#include <Texture.h>
#include "HealthUI.h"
#include "TextureManager.h"


HudManager::HudManager(const TextureManager* textureManager):
	m_HealthUI{new HealthUI(Point2f(0,400),textureManager)}
{
}

HudManager::~HudManager()
{
	delete m_HealthUI;
}

void HudManager::Update(float elapsedSec)
{
	m_HealthUI->Update(elapsedSec);
}

void HudManager::Draw() const
{
	m_HealthUI->Draw();

}

void HudManager::SetHealth(int health)
{
	m_HealthUI->SetHealth(health);
}
