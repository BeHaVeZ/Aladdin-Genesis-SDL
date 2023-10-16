#include "pch.h"
#include "TextureManager.h"
#include <fstream>
#include <string>
#include <sstream>


TextureManager::TextureManager()
{
    InitPlayerTextures();
    InitProjectileTextures();
    InitEnemyTextures();
    InitPickUpTextures();
    InitLevelTextures();
    InitParticleTextures();
    InitUITextures();
}

TextureManager::~TextureManager()
{
    // Clean up textures
    for (std::pair<const TextureManager::TextureKey, Texture*>& pair : m_TextureMaps) {
        delete pair.second;
    }
}

#pragma region InitFunctionDefinitions
void TextureManager::InitPlayerTextures()
{
    AddTexture(TextureKey::playerTxr, new Texture("Textures/Player/Genesis 32X SCD - Aladdin - Aladdin.png"));
}

void TextureManager::InitProjectileTextures()
{
    AddTexture(TextureKey::KnifeProjectileTxr, new Texture("Textures/Projectiles/Knife.png"));
    AddTexture(TextureKey::PotProjectileTxr, new Texture("Textures/Projectiles/Pot.png"));
    AddTexture(TextureKey::SpitProjectileTxr, new Texture("Textures/Projectiles/Spit.png"));
    AddTexture(TextureKey::AppleProjectileTxr, new Texture("Textures/Projectiles/Apple.png"));
}

void TextureManager::InitEnemyTextures()
{
    AddTexture(TextureKey::RangedEnemyTxr, new Texture("Textures/Enemies/RangedEnemy.png"));
    AddTexture(TextureKey::GuardTxr, new Texture("Textures/Enemies/Guard.png"));
    AddTexture(TextureKey::PotThrowerTxr, new Texture("Textures/Enemies/PotThrower.png"));
    AddTexture(TextureKey::CasketEnemyTxr, new Texture("Textures/Enemies/Casket.png"));
}

void TextureManager::InitPickUpTextures()
{
    AddTexture(TextureKey::HealthPickUpTxr, new Texture("Textures/PickUps/HealthPickUp.png"));
    AddTexture(TextureKey::ApplePickUpTxr, new Texture("Textures/PickUps/PickUp_Apple.png"));
}

void TextureManager::InitLevelTextures()
{
    AddTexture(TextureKey::BackGroundTxr, new Texture("Textures/Level/background.png"));
    AddTexture(TextureKey::BGP_1Txr, new Texture("Textures/Level/bgp_1.png"));
    AddTexture(TextureKey::BGP_2Txr, new Texture("Textures/Level/bgp_2.png"));
    AddTexture(TextureKey::BGP_3Txr, new Texture("Textures/Level/bgp_3.png"));
    AddTexture(TextureKey::BGP_4Txr, new Texture("Textures/Level/bgp_4.png"));
    AddTexture(TextureKey::BGP_5Txr, new Texture("Textures/Level/bgp_5.png"));
    AddTexture(TextureKey::BGP_6Txr, new Texture("Textures/Level/bgp_6.png"));
    AddTexture(TextureKey::BGP_7Txr, new Texture("Textures/Level/bgp_7.png"));
    AddTexture(TextureKey::BGP_8Txr, new Texture("Textures/Level/bgp_8.png"));
    AddTexture(TextureKey::BGP_9Txr, new Texture("Textures/Level/bgp_9.png"));
    AddTexture(TextureKey::BlockTxr, new Texture("Textures/Level/block.png"));
    AddTexture(TextureKey::ForeGround_1Txr, new Texture("Textures/Level/forground_01.png"));
    AddTexture(TextureKey::ForeGround_2Txr, new Texture("Textures/Level/forground_02.png"));
    AddTexture(TextureKey::ForeGround_3Txr, new Texture("Textures/Level/forground_03.png"));
    AddTexture(TextureKey::CamelTxr, new Texture("Textures/Level/Genesis 32X SCD - Aladdin - Camel.png"));
    AddTexture(TextureKey::SKY_1Txr, new Texture("Textures/Level/sky_1.png"));
    AddTexture(TextureKey::SKY_2Txr, new Texture("Textures/Level/sky_2.png"));
    AddTexture(TextureKey::SKY_3Txr, new Texture("Textures/Level/sky_3.png"));
    AddTexture(TextureKey::SKY_4Txr, new Texture("Textures/Level/sky_4.png"));
}

void TextureManager::InitParticleTextures()
{
    AddTexture(TextureKey::ApplePickUpEffectTxr, new Texture("Textures/Particles/applePickUpEffect.png"));
    AddTexture(TextureKey::AppleSplashTxr, new Texture("Textures/Particles/appleSplash.png"));
    AddTexture(TextureKey::EnemyDeathExplosionTxr, new Texture("Textures/Particles/enemyDeathExplosion.png"));
    AddTexture(TextureKey::FireTxr, new Texture("Textures/Particles/Fire.png"));
    AddTexture(TextureKey::HealthPickUpEffectTxr, new Texture("Textures/Particles/HealthPickUpEffect.png"));
    AddTexture(TextureKey::PotExplosionTxr, new Texture("Textures/Particles/potExplosion.png"));
}
void TextureManager::InitUITextures()
{
    AddTexture(TextureKey::GenieGameOverTxr, new Texture("Textures/UI/DeathSprite.png"));
    AddTexture(TextureKey::GameOverScreen, new Texture("Textures/UI/GameOverScreen.png"));
    AddTexture(TextureKey::GameOverScreenYes, new Texture("Textures/UI/GameOverScreenYes.png"));
    AddTexture(TextureKey::GameOverScreenNo, new Texture("Textures/UI/GameOverScreenNo.png"));
    AddTexture(TextureKey::GameFinished, new Texture("Textures/UI/LevelComplete.png"));
    AddTexture(TextureKey::HealthUITxr, new Texture("Textures/UI/HealthUI.png"));
}
#pragma endregion