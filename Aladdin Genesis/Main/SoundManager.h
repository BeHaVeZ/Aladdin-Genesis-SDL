#pragma once

class SoundStream;
class SoundEffect;


class SoundManager final
{
public:
	enum class SoundEffects {
		PickUpSound = 1,
		AppleSplat,
		HighSword,
		LowSword,
		Throw,
		CamelSpit,
		FallingPlatform,
		ClayPot,
		EnemyDamaged,
		WallHit,
		FireFromCoal,
		PlayerDamaged,
		GuardDamaged,
		GenieSpawn,
		GameOver,
		GameOverMusic,
		ExtraHealth,
		GuardsPants,
		GuardHit1
	};

	SoundManager();
	~SoundManager();


	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;

	void PlayOST() const;
	void Stop() const;
	void Pause() const;
	void Resume() const;
	void MuteSound();
	void AddVolume();
	void SubVolume() ;
	void PlaySoundEffect(SoundEffects soundEffectToPlay) const;

private:

	bool m_IsMuted;

	int m_InitialVolume;
	int m_VolumeJump;

	SoundEffects m_EffectToPlay;

	SoundStream* m_Level01_ost;
	SoundStream* m_BossLevel01_ost;

	SoundEffect* m_PickUpSound;
	SoundEffect* m_AppleSplat;
	SoundEffect* m_HighSword;
	SoundEffect* m_LowSword;
	SoundEffect* m_Throw;
	SoundEffect* m_CamelSpitting;
	SoundEffect* m_FallingPlatform;
	SoundEffect* m_ClayPot;
	SoundEffect* m_EnemyDamaged;
	SoundEffect* m_WallHit;
	SoundEffect* m_FireFromCoal;
	SoundEffect* m_PlayerDamaged;
	SoundEffect* m_GuardDamaged;
	SoundEffect* m_GenieSpawn;
	SoundEffect* m_GameOver;
	SoundEffect* m_GameOverMusic;
	SoundEffect* m_ExtraHealth;
	SoundEffect* m_GuardsPants;
	SoundEffect* m_GuardHit1;

	void InitSoundEffects();
};

