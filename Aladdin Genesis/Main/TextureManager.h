#pragma once
#include <unordered_map>
#include <Texture.h>

class TextureManager final {
public:
    //De key die gebruikt wordt om de correcte textureptr te returnen
    //Txr in general personal naming preference (Txr --> Texture)
    enum class TextureKey {
        //Player
        playerTxr,
        //Enemies
        GuardTxr,
        PotThrowerTxr,
        RangedEnemyTxr,
        CasketEnemyTxr,
        //Particles
        ApplePickUpEffectTxr,
        AppleSplashTxr,
        EnemyDeathExplosionTxr,
        FireTxr,
        HealthPickUpEffectTxr,
        PotExplosionTxr,
        //PickUps
        HealthPickUpTxr,
        ApplePickUpTxr,
        //Projectiles
        AppleProjectileTxr,
        KnifeProjectileTxr,
        PotProjectileTxr,
        SpitProjectileTxr,
        //UI
        HealthTxr,
        //Level
        BackGroundTxr,
        ///BGP --> BackGroundParallax 1/2/3...
        BGP_1Txr,
        BGP_2Txr,
        BGP_3Txr,
        BGP_4Txr,
        BGP_5Txr,
        BGP_6Txr,
        BGP_7Txr,
        BGP_8Txr,
        BGP_9Txr,
        ForeGround_1Txr,
        ForeGround_2Txr,
        ForeGround_3Txr,
        SKY_1Txr,
        SKY_2Txr,
        SKY_3Txr,
        SKY_4Txr,
        BlockTxr,
        CamelTxr,
        //UI
        GenieGameOverTxr,
        GameOverScreen,
        GameOverScreenYes,
        GameOverScreenNo,
        GameFinished,
        HealthUITxr

    };
    explicit TextureManager();
    ~TextureManager();
    TextureManager(const TextureManager& other) = delete;
    TextureManager& operator=(const TextureManager& other) = delete;
    TextureManager(TextureManager&& other) = delete;
    TextureManager& operator=(TextureManager&& other) = delete;


    void AddTexture(TextureKey key, Texture* texture) {
        m_TextureMaps[key] = texture;
    }
    const Texture* GetTextureReference(TextureKey key) const{
        return m_TextureMaps.at(key);
    }
protected:
    std::unordered_map<TextureKey, Texture*> m_TextureMaps;
private:
    void InitPlayerTextures();
    void InitProjectileTextures();
    void InitEnemyTextures();
    void InitPickUpTextures();
    void InitLevelTextures();
    void InitParticleTextures();
    void InitUITextures();
};