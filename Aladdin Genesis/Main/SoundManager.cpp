#include "pch.h"
#include "SoundManager.h"
#include <SoundEffect.h>
#include <SoundStream.h>

SoundManager::SoundManager() :
	m_IsMuted{ false },
	m_InitialVolume{ 100 },
	m_VolumeJump{ 5 }
{
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);
	m_Level01_ost->SetVolume(m_InitialVolume);
	m_Level01_ost = new SoundStream("sound/AladdinOst.mp3");
	InitSoundEffects();
}

SoundManager::~SoundManager()
{
	delete m_Level01_ost;
	delete m_PickUpSound;
	delete m_AppleSplat;
	delete m_HighSword;
	delete m_LowSword;
	delete m_Throw;
	delete m_CamelSpitting;
	delete m_FallingPlatform;
	delete m_ClayPot;
	delete m_EnemyDamaged;
	delete m_WallHit;
	delete m_FireFromCoal;
	delete m_PlayerDamaged;
	delete m_GuardDamaged;
	delete m_GenieSpawn;
	delete m_GameOver;
	delete m_GameOverMusic;
	delete m_ExtraHealth;
	delete m_GuardsPants;
	delete m_GuardHit1;
}

void SoundManager::PlayOST() const
{
	m_Level01_ost->Play(1);
}

void SoundManager::Stop() const
{
	m_Level01_ost->Stop();
}

void SoundManager::Pause() const
{
	m_Level01_ost->Pause();
}

void SoundManager::Resume() const
{
	m_Level01_ost->Resume();
}

void SoundManager::MuteSound()
{
	if (m_IsMuted == false)
	{
		m_Level01_ost->SetVolume(0);
		m_IsMuted = true;
	}
	else {
		m_Level01_ost->SetVolume(m_InitialVolume);
		m_IsMuted = false;
	}
}

void SoundManager::AddVolume() 
{
	if (m_InitialVolume < 100)
	{
		m_InitialVolume += m_VolumeJump;
		m_Level01_ost->SetVolume(m_InitialVolume);
	}
}

void SoundManager::SubVolume()
{
	if (m_InitialVolume < 0)
	{
		m_InitialVolume = 0;
	}
	if (m_InitialVolume > 0)
	{
		m_InitialVolume -= m_VolumeJump;
		m_Level01_ost->SetVolume(m_InitialVolume);
	}
}

void SoundManager::PlaySoundEffect(SoundEffects soundEffectToPlay) const
{
	if (m_IsMuted == false)
	{
		switch (soundEffectToPlay)
		{
		case SoundManager::SoundEffects::PickUpSound:
			m_PickUpSound->SetVolume(m_InitialVolume);
			m_PickUpSound->Play(0);
			break;
		case SoundManager::SoundEffects::AppleSplat:
			m_AppleSplat->SetVolume(m_InitialVolume);
			m_AppleSplat->Play(0);
			break;
		case SoundManager::SoundEffects::HighSword:
			m_HighSword->SetVolume(m_InitialVolume);
			m_HighSword->Play(0);
			break;
		case SoundManager::SoundEffects::LowSword:
			m_LowSword->SetVolume(m_InitialVolume);
			m_LowSword->Play(0);
			break;
		case SoundEffects::Throw:
			m_Throw->SetVolume(m_InitialVolume);
			m_Throw->Play(0);
			break;
		case SoundEffects::CamelSpit:
			m_CamelSpitting->SetVolume(m_InitialVolume);
			m_CamelSpitting->Play(0);
			break;
		case SoundEffects::FallingPlatform:
			m_FallingPlatform->SetVolume(m_InitialVolume);
			m_FallingPlatform->Play(0);
			break;
		case SoundEffects::ClayPot:
			m_ClayPot->SetVolume(m_InitialVolume);
			m_ClayPot->Play(0);
			break;
		case SoundEffects::WallHit:
			m_WallHit->SetVolume(m_InitialVolume);
			m_WallHit->Play(0);
			break;
		case SoundEffects::EnemyDamaged:
			m_EnemyDamaged->SetVolume(m_InitialVolume);
			m_EnemyDamaged->Play(0);
			break;
		case SoundEffects::FireFromCoal:
			m_FireFromCoal->SetVolume(m_InitialVolume);
			m_FireFromCoal->Play(0);
			break;
		case SoundEffects::PlayerDamaged:
			m_PlayerDamaged->SetVolume(m_InitialVolume);
			m_PlayerDamaged->Play(0);
			break;
		case SoundEffects::GuardDamaged:
			m_GuardDamaged->SetVolume(m_InitialVolume);
			m_GuardDamaged->Play(0);
			break;
		case SoundEffects::GenieSpawn:
			m_GenieSpawn->SetVolume(m_InitialVolume);
			m_GenieSpawn->Play(0);
			break;
		case SoundEffects::GameOver:
			m_GameOver->SetVolume(m_InitialVolume);
			m_GameOver->Play(0);
			break;
		case SoundEffects::GameOverMusic:
			m_GameOverMusic->SetVolume(m_InitialVolume);
			m_GameOverMusic->Play(0);
			break;
		case SoundEffects::ExtraHealth:
			m_ExtraHealth->SetVolume(m_InitialVolume);
			m_ExtraHealth->Play(0);
			break;
		case SoundEffects::GuardsPants:
			m_GuardsPants->SetVolume(m_InitialVolume);
			m_GuardsPants->Play(0);
			break;
		case SoundEffects::GuardHit1:
			m_GuardHit1->SetVolume(m_InitialVolume);
			m_GuardHit1->Play(0);
			break;

		}
	}
}

void SoundManager::InitSoundEffects()
{
	m_PickUpSound = new SoundEffect("sound/Effects/Apple Collect.wav");
	m_AppleSplat = new SoundEffect("sound/Effects/Apple Splat.wav");
	m_HighSword = new SoundEffect("sound/Effects/High Sword.wav");
	m_LowSword = new SoundEffect("sound/Effects/Low Sword.wav");
	m_Throw = new SoundEffect("sound/Effects/Object Throw.wav");
	m_CamelSpitting = new SoundEffect("sound/Effects/Camel Spit.wav");
	m_FallingPlatform = new SoundEffect("sound/Effects/Starting Gun.wav");
	m_ClayPot = new SoundEffect("sound/Effects/Clay Pot.wav");
	m_EnemyDamaged = new SoundEffect("sound/Effects/Eeeh.wav");
	m_WallHit = new SoundEffect("sound/Effects/Wall Hit 2.wav");
	m_FireFromCoal = new SoundEffect("sound/Effects/Fire from Coal.wav");
	m_PlayerDamaged = new SoundEffect("sound/Effects/Aaah.wav");
	m_GuardDamaged = new SoundEffect("sound/Effects/Iiee.wav");
	m_GenieSpawn = new SoundEffect("sound/Effects/Genie Fumes.wav");
	m_GameOver = new SoundEffect("sound/Effects/Aladdin-Game_over.mp3");
	m_GameOverMusic = new SoundEffect("sound/Effects/Aladdin - One Jump Ahead.mp3");
	m_ExtraHealth = new SoundEffect("sound/Effects/Extra Health.wav");
	m_GuardsPants = new SoundEffect("sound/Effects/Guard Pants.wav");
	m_GuardHit1 = new SoundEffect("sound/Effects/Guard Hit 1.wav");
}