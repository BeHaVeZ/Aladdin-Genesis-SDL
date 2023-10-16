#include "pch.h"
#include <Texture.h>
#include <fstream>
#include <vector>
#include <Vector2f.h>
#include <utils.h>
#include <iostream>

#include "Level.h"
#include "SVGParser.h"
#include "Game.h"

//Projectile
#include "Projectile.h"
//Particles
#include "Particle.h"
//Level
#include "Rope.h"
#include "Platform.h"
#include "LavaPool.h"
#include "Player.h"
#include "FallingPlatform.h"
#include "Camel.h"
#include "Camera.h"
//UI
#include "HealthUI.h"
//Enemies
#include "Enemy.h"
#include "RangedEnemy.h"
#include "PotThrower.h"
#include "Guard.h"
#include "CasketEnemy.h"
//PickUps
#include "PickUp.h"
#include "ApplePickUp.h"
#include "HealthPickUp.h"
//Managers
#include "TextureManager.h"
#include "ParticleManager.h"
#include "HudManager.h"
#include "SoundManager.h"



#define SoundEffect SoundManager::SoundEffects
#define ParticleEffect ParticleManager::ParticleEffect


Level::Level()
{
	m_pTextureManager = new TextureManager();
	m_LevelSoundManager = new SoundManager();
	m_ParticleManager = new ParticleManager(m_pTextureManager);
	m_HudManager = new HudManager(m_pTextureManager);

	std::vector<std::vector<Point2f>> allVertices{};
	SVGParser::GetVerticesFromSvgFile("map/level.svg", allVertices);
	m_Vertices = allVertices[0];

	m_Boundaries = Rectf(0, 0, m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BackGroundTxr)->GetWidth(), m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BackGroundTxr)->GetWidth());

	m_Player = new Player(*this,*m_pTextureManager);

	m_Camel = new Camel(Point2f(1065, 15), m_AliveProjectiles,*m_pTextureManager);

	m_Finish = Rectf(3328,354,30,30);
	m_IsFinished = false;

	InitEnemies();
	InitLavaPools();
	InitRopes();
	InitFallingPlatforms();
	InitPlatforms();
	InitPickUpPositions();
	m_LevelSoundManager->PlayOST();
}


Level::~Level()
{
	delete m_Player;
	delete m_Camel;
	delete m_LevelSoundManager;
	delete m_ParticleManager;
	delete m_HudManager;
	delete m_pTextureManager;
	for (LavaPool* lavaPool : m_Lavapools)
	{
		delete lavaPool;
	}

	for (Rope* rope : m_Ropes)
	{
		delete rope;
	}

	for (FallingPlatform* fallingPlatform : m_FallingPlatforms)
	{
		delete fallingPlatform;
	}

	for (Platform* platform : m_Platforms)
	{
		delete platform;
	}

	for (Enemy* enemy : m_AliveEnemies)
	{
		delete enemy;
	}

	for (Projectile* projectile : m_AliveProjectiles)
	{
		delete projectile;
	}

	for (PickUp* pickUp : m_PickUps)
	{
		delete pickUp;
	}

}

void Level::Update(float elapsedSec)
{
	if (utils::IsOverlapping(*m_Player->GetHitBox(),m_Finish))
	{
		m_IsFinished = true;
	}
	m_Player->Update(elapsedSec, *this);
	m_LevelElapsedSec = elapsedSec;
	m_Camel->Update(elapsedSec);
	m_ParticleManager->Update(elapsedSec);
	m_HudManager->Update(elapsedSec);

	m_HudManager->SetHealth(m_Player->GetPlayerHealth());

	for (FallingPlatform* fallingPlatform : m_FallingPlatforms)
	{
		fallingPlatform->Update(elapsedSec);
	}

	//Checkt of de guard over lava loopt en of de speler geraakt is
	for (Enemy* enemy : m_AliveEnemies)
	{
		enemy->Update(elapsedSec, *m_Player->GetHitBox());
		if (enemy->IsGuard())
		{
			for (LavaPool* lavaPool : m_Lavapools)
			{
				if (utils::IsOverlapping(enemy->GetHitBox(), lavaPool->GetLavaRect()))
				{
					static_cast<Guard*>(enemy)->IsWalkingOverLava(true);
					break;
				}
				else {
					static_cast<Guard*>(enemy)->IsWalkingOverLava(false);
				}
			}
			if (utils::IsOverlapping(*m_Player->GetHitBox(), static_cast<Guard*>(enemy)->GetAttackZone()))
			{
				m_Player->Damage();
			}
		}
		if (enemy->IsCasketEnemy())
		{
			if (utils::IsOverlapping(*m_Player->GetHitBox(), static_cast<CasketEnemy*>(enemy)->GetAttackZone()))
			{
				m_Player->Damage();
			}
		}
	}
	for (Projectile* projectile : m_AliveProjectiles)
	{
		projectile->Update(elapsedSec);
	}
	for (PickUp* pickUp : m_PickUps)
	{
		pickUp->Update(elapsedSec);
	}
	for (LavaPool* lavaPool : m_Lavapools)
	{
		lavaPool->Update(elapsedSec);
	}
	//Hier wordt gecheckt voor elke pickup of er collision is
	for (int index = (int)m_PickUps.size() - 1; index >= 0; index--)
	{
		//checkt of er een pickup geraakt wordt;
		if (m_PickUps[index]->IsHit(*m_Player->GetHitBox()) == true)
		{
			//welke pick up is geraakt?]
			//Appel
			if (m_PickUps[index]->IsApplePickUp())
			{
				m_ParticleManager->PlayParticle(ParticleEffect::appleSparkles, Point2f(m_PickUps[index]->GetHitBox().left, m_PickUps[index]->GetHitBox().bottom));
				delete m_PickUps[index];
				m_PickUps[index] = m_PickUps[m_PickUps.size() - 1];
				m_PickUps.pop_back();
				m_LevelSoundManager->PlaySoundEffect(SoundEffect::PickUpSound);
			}
			//HealthPU
			else if (m_PickUps[index]->IsHealthPickUp())
			{
				m_ParticleManager->PlayParticle(ParticleEffect::healthSparkles, Point2f(m_PickUps[index]->GetHitBox().left, m_PickUps[index]->GetHitBox().bottom));
				delete m_PickUps[index];
				m_PickUps[index] = m_PickUps[m_PickUps.size() - 1];
				m_PickUps.pop_back();
				m_LevelSoundManager->PlaySoundEffect(SoundEffect::ExtraHealth);
				m_Player->AddHealth();
			}
		}
	}
	//Checkt Appel Collision met enemies
	for (int i = (int)m_AliveProjectiles.size() - 1; i >= 0; --i)
	{
		m_AliveProjectiles[i]->Update(elapsedSec);
		if (m_AliveProjectiles[i]->CanHurtEnemy())
		{
			for (int index = (int)m_AliveEnemies.size() - 1; index >= 0; index--)
			{
				if (utils::IsOverlapping(m_AliveEnemies[index]->GetHitBox(), m_AliveProjectiles[i]->GetHitBox()) and m_AliveEnemies[index]->IsPotThrower() == false)
				{
					m_ParticleManager->PlayParticle(ParticleEffect::appleSplash, Point2f(m_AliveProjectiles[i]->GetHitBox().left, m_AliveProjectiles[i]->GetHitBox().bottom));
					delete m_AliveProjectiles[i];
					m_AliveProjectiles[i] = m_AliveProjectiles[m_AliveProjectiles.size() - 1];
					m_AliveProjectiles.pop_back();

					m_AliveEnemies[index]->ReceiveDamage();

					m_LevelSoundManager->PlaySoundEffect(SoundEffect::AppleSplat);
					return;
				}
			}
		}

		//Checkt of een Projectiel collide met de level of de speler
		if (IsColliding(m_AliveProjectiles[i]->GetHitBox()))
		{
			if (m_AliveProjectiles[i]->IsAppleProjectile())
			{
				m_ParticleManager->PlayParticle(ParticleEffect::appleSplash, Point2f(m_AliveProjectiles[i]->GetHitBox().left, m_AliveProjectiles[i]->GetHitBox().bottom));
				delete m_AliveProjectiles[i];
				m_AliveProjectiles[i] = m_AliveProjectiles[m_AliveProjectiles.size() - 1];
				m_AliveProjectiles.pop_back();
				m_LevelSoundManager->PlaySoundEffect(SoundEffect::AppleSplat);
			}
			else if (m_AliveProjectiles[i]->IsPotProjectile()) {
				m_ParticleManager->PlayParticle(ParticleEffect::potExplosion, Point2f(m_AliveProjectiles[i]->GetHitBox().left, m_AliveProjectiles[i]->GetHitBox().bottom));
				delete m_AliveProjectiles[i];
				m_AliveProjectiles[i] = m_AliveProjectiles[m_AliveProjectiles.size() - 1];
				m_AliveProjectiles.pop_back();
				m_LevelSoundManager->PlaySoundEffect(SoundEffect::ClayPot);
			}
			else {
				delete m_AliveProjectiles[i];
				m_AliveProjectiles[i] = m_AliveProjectiles[m_AliveProjectiles.size() - 1];
				m_AliveProjectiles.pop_back();
				m_LevelSoundManager->PlaySoundEffect(SoundEffect::WallHit);
			}
		}
	}

	//Checkt de swordRange van de speler en of dat die collide met de enemies
	for (int index = (int)m_AliveEnemies.size() - 1; index >= 0; index--)
	{
		if (utils::IsOverlapping(*m_Player->GetAttackBox(), m_AliveEnemies[index]->GetHitBox()) and m_AliveEnemies[index]->IsPotThrower() == false)
		{
			m_AliveEnemies[index]->ReceiveDamage();
			break;
		}
	}

	//Checkt of een enemy projectile collide met de speler
	for (int i = (int)m_AliveProjectiles.size() - 1; i >= 0; i--)
	{
		if (utils::IsOverlapping(m_AliveProjectiles[i]->GetHitBox(), *m_Player->GetHitBox()) and m_AliveProjectiles[i]->CanHurtEnemy() == false and m_Player->CanBeDamaged() == true)
		{
			if (m_AliveProjectiles[i]->IsAppleProjectile())
			{
				m_ParticleManager->PlayParticle(ParticleEffect::appleSplash, Point2f(m_AliveProjectiles[i]->GetHitBox().left, m_AliveProjectiles[i]->GetHitBox().bottom));
				delete m_AliveProjectiles[i];
				m_AliveProjectiles[i] = m_AliveProjectiles[m_AliveProjectiles.size() - 1];
				m_AliveProjectiles.pop_back();
				m_LevelSoundManager->PlaySoundEffect(SoundEffect::AppleSplat);
			}
			else if (m_AliveProjectiles[i]->IsPotProjectile()) {
				m_ParticleManager->PlayParticle(ParticleEffect::potExplosion, Point2f(m_AliveProjectiles[i]->GetHitBox().left, m_AliveProjectiles[i]->GetHitBox().bottom));
				delete m_AliveProjectiles[i];
				m_AliveProjectiles[i] = m_AliveProjectiles[m_AliveProjectiles.size() - 1];
				m_AliveProjectiles.pop_back();
				m_LevelSoundManager->PlaySoundEffect(SoundEffect::ClayPot);
			}
			else {
				delete m_AliveProjectiles[i];
				m_AliveProjectiles[i] = m_AliveProjectiles[m_AliveProjectiles.size() - 1];
				m_AliveProjectiles.pop_back();
				m_LevelSoundManager->PlaySoundEffect(SoundEffect::WallHit);
			}
			m_Player->Damage();
		}
	}

	//checkt of er enemies zijn met health 0 om die dan te verwijderen
	for (int i = (int)m_AliveEnemies.size() - 1; i >= 0; i--)
	{
		if (m_AliveEnemies[i]->GetCurrentHealth() == 0)
		{
			m_ParticleManager->PlayParticle(ParticleEffect::enemyDeathExplosion, Point2f(m_AliveEnemies[i]->GetHitBox().left, m_AliveEnemies[i]->GetHitBox().bottom));
			delete m_AliveEnemies[i];
			m_AliveEnemies[i] = m_AliveEnemies[m_AliveEnemies.size() - 1];
			m_AliveEnemies.pop_back();

		}
	}

	//Log("X: " << m_Player->GetHitBox()->left << " Y: " << m_Player->GetHitBox()->bottom);
}

void Level::DrawBackGround(Camera* camera) const
{
	glPushMatrix();
	{
		glScalef(1, 2, 1);

		glPushMatrix();
		{
			glTranslatef(camera->GetCameraPos().x * .1f, 0, 0);
			glScalef(5, .5f, 1);
			m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BGP_1Txr)->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(camera->GetCameraPos().x * .2f, 0, 0);
			glScalef(5, .5f, 1);
			m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BGP_2Txr)->Draw();
		}
		glPopMatrix();


		glPushMatrix();
		{
			glTranslatef(camera->GetCameraPos().x * .3f, 0, 0);
			glScalef(5, .5f, 1);
			m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BGP_3Txr)->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(camera->GetCameraPos().x * .4f, 0, 0);
			glScalef(5, .5f, 1);
			m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BGP_2Txr)->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(camera->GetCameraPos().x * .5f, 0, 0);
			glScalef(5, .5f, 1);
			m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BGP_4Txr)->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(camera->GetCameraPos().x * .6f, 0, 0);
			glScalef(5, .5f, 1);
			m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BGP_5Txr)->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(camera->GetCameraPos().x * .7f, 0, 0);
			glScalef(5, .5f, 1);
			m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BGP_6Txr)->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(camera->GetCameraPos().x * .8f, 0, 0);
			glScalef(5, .5f, 1);
			m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BGP_7Txr)->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(camera->GetCameraPos().x * .9f, 0, 0);
			glScalef(5, .5f, 1);
			m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BGP_8Txr)->Draw();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(camera->GetCameraPos().x * .95f, 0, 0);
			glScalef(5, .5f, 1);
			m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BGP_9Txr)->Draw();
		}
		glPopMatrix();
	}
	glPopMatrix();

	m_pTextureManager->GetTextureReference(TextureManager::TextureKey::BackGroundTxr)->Draw();

}


void Level::DrawForeGround() const
{
	//utils::DrawPolygon(m_Vertices);
	m_Camel->Draw();


	for (Enemy* enemy : m_AliveEnemies)
	{
		enemy->Draw();
	}

	for (Projectile* projectile : m_AliveProjectiles)
	{
		projectile->Draw();
	}

	for (PickUp* pickUp : m_PickUps)
	{
		pickUp->Draw();
	}

	for (FallingPlatform* fallingPlatform : m_FallingPlatforms)
	{
		fallingPlatform->Draw();
	}



	m_ParticleManager->Draw();
	m_Player->Draw();
	m_pTextureManager->GetTextureReference(TextureManager::TextureKey::ForeGround_1Txr)->Draw((Point2f(831, 0)));
	m_pTextureManager->GetTextureReference(TextureManager::TextureKey::ForeGround_2Txr)->Draw((Point2f(1662, 0)));
	m_pTextureManager->GetTextureReference(TextureManager::TextureKey::ForeGround_3Txr)->Draw((Point2f(2493, 0)));
}

void Level::DrawHud() const
{
	m_HudManager->Draw();
}

//deze functie is gemaakt voor appels om te checken of de appel collide met de level
bool Level::IsColliding(const Rectf& actorShape) const
{
	utils::HitInfo hit;
	if (utils::Raycast(m_Vertices,
		Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height),
		Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom - 1), hit)
		or
		utils::Raycast(m_Vertices,
			Point2f(actorShape.left - 1, (actorShape.bottom + actorShape.height / 2) + 1),
			Point2f(actorShape.left + actorShape.width, (actorShape.bottom + actorShape.height / 2) - 1), hit))
	{
		return true;
	}
	return false;
}


void Level::HandleCollision() const
{
	utils::HitInfo hit{  };
	if (utils::Raycast(m_Vertices,
		Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom + m_Player->GetShape()->height * .6f),
		Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom), hit))
	{
		//checkt of je aan het springen bent en iets raakt terwijl je in de lucht bent
		if (m_Player->GetVelocity()->y > 1 and utils::Raycast(m_Vertices,
			Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom + m_Player->GetShape()->height * .6f),
			Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom + m_Player->GetShape()->height / 2), hit))
		{
			m_Player->GetShape()->bottom = hit.intersectPoint.y - m_Player->GetShape()->height * .6f;
			m_Player->GetVelocity()->y = 0;
		}
		//checkt gewoon collision op de grond en zorgt ervoor dat de player niet door de grond valt
		else {
			m_Player->GetShape()->bottom = hit.intersectPoint.y;
			m_Player->GetVelocity()->y = 0;
		}
	}

	//checkt of de speler op de platform is
	for (Platform* platform : m_Platforms)
	{
		if (utils::Raycast(platform->GetPlatformPoints(),
			Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom + m_Player->GetShape()->height * .6f),
			Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom), hit) and m_Player->GetVelocity()->y < 0)
		{
			m_Player->GetShape()->bottom = hit.intersectPoint.y;
			m_Player->GetVelocity()->y = 0;
		}
	}

	//checkt of er collision is met de camel in de level
	if (utils::Raycast(m_Camel->GetCamelCollisionPoints(),
		Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom + m_Player->GetShape()->height * .6f),
		Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom), hit) and m_Player->GetVelocity()->y < 0)
	{
		float bounceHeight{ 300 };
		m_Camel->Shoot();
		m_LevelSoundManager->PlaySoundEffect(SoundEffect::CamelSpit);
		m_Player->GetVelocity()->y = 0;
		m_Player->GetVelocity()->y += bounceHeight;
	}

	//checkt of de speler de muur raakt zodat hij niet shift door de muren
	if (utils::Raycast(m_Vertices,
		Point2f(m_Player->GetShape()->left - 1, (m_Player->GetShape()->bottom + m_Player->GetShape()->height / 2)),
		Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width, (m_Player->GetShape()->bottom + m_Player->GetShape()->height / 2)), hit) and m_Player->GetVelocity()->x < 0)
	{
		m_Player->GetShape()->left = hit.intersectPoint.x;
		m_Player->GetVelocity()->x = 0;
	}
	else
		if (utils::Raycast(m_Vertices,
			Point2f(m_Player->GetShape()->left, (m_Player->GetShape()->bottom + m_Player->GetShape()->height / 2)),
			Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width, (m_Player->GetShape()->bottom + m_Player->GetShape()->height / 2)), hit) and m_Player->GetVelocity()->x > 0)
		{
			m_Player->GetShape()->left = hit.intersectPoint.x - m_Player->GetShape()->width;
			m_Player->GetVelocity()->x = 0;
		}
	for (Rope* rope : m_Ropes)
	{
		if (rope->IsColliding(*m_Player->GetShape()) == true and m_Player->GetVelocity()->y < 0)
		{
			m_Player->SetClimbingState(true);
			rope->HandleCollision(*m_Player->GetShape(), *m_Player->GetVelocity());
		}
	}

	for (FallingPlatform* fallingPlatform : m_FallingPlatforms)
	{
		if (utils::Raycast(fallingPlatform->GetPlatformPoints(),
			Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom + m_Player->GetShape()->height * .6f),
			Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom), hit) and m_Player->GetVelocity()->y < 0)
		{
			fallingPlatform->SetFalling();
			m_Player->GetShape()->bottom = hit.intersectPoint.y;
			m_Player->GetVelocity()->y = 0;
			m_LevelSoundManager->PlaySoundEffect(SoundEffect::FallingPlatform);
		}
	}

	for (LavaPool* lavaPool : m_Lavapools)
	{
		if (utils::Raycast(lavaPool->GetPlatformPoints(),
			Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom + m_Player->GetShape()->height * .6f),
			Point2f(m_Player->GetShape()->left + m_Player->GetShape()->width / 2, m_Player->GetShape()->bottom), hit) and lavaPool->IsReadyToExplode())
		{
			lavaPool->HasCollided(hit.intersectPoint);
			m_LevelSoundManager->PlaySoundEffect(SoundEffect::FireFromCoal);
		}

		if (utils::IsOverlapping(*m_Player->GetHitBox(), lavaPool->GetHitBox()) and m_Player->CanBeDamaged() == true)
		{
			m_Player->Damage();
		}
	}
}

bool Level::IsOnGround(const Rectf& actorShape) const
{
	utils::HitInfo hit;
	if (utils::Raycast(m_Vertices,
		Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2),
		Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom - 1), hit)
		)
	{
		return true;
	}

	for (Platform* platform : m_Platforms)
	{
		if (utils::Raycast(platform->GetPlatformPoints(),
			Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2),
			Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom), hit))
		{
			return true;
		}
	}
	for (FallingPlatform* m_FallingPlatform : m_FallingPlatforms)
	{
		if (utils::Raycast(m_FallingPlatform->GetPlatformPoints(),
			Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2),
			Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom), hit))
		{
			return true;
		}
	}
	return false;
}

bool Level::IsAgainstWall(const Rectf& actorShape) const
{
	utils::HitInfo hit;
	if (utils::Raycast(m_Vertices,
		Point2f(actorShape.left - 1, (actorShape.bottom + actorShape.height / 2) + 1),
		Point2f(actorShape.left + actorShape.width, (actorShape.bottom + actorShape.height / 2) - 1), hit))
	{
		return true;
	}
	return false;
}

bool Level::IsFinished() const
{
	return m_IsFinished;
}


const Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

std::vector<Projectile*>* Level::GetLevelProjectiles()
{
	return &m_AliveProjectiles;
}

SoundManager* Level::GetLevelSoundManager() const
{
	return m_LevelSoundManager;
}

const TextureManager* Level::GetLevelTextureManager() const
{
	return m_pTextureManager;
}

void Level::InitLavaPools()
{
	std::ifstream myFileStream("LavaPoolPositions.txt");
	if (!myFileStream.is_open())
	{
		Log("Positions of apples failed to load!");
		throw std::invalid_argument("Failed to load the textfile with the pickup positions");
	}
	int X, Y;
	std::string myString, line;


	//Initializing applepickups in the game
	while (std::getline(myFileStream, line))
	{
		std::stringstream stream(line);
		std::getline(stream, myString, ',');
		X = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ' ');
		Y = std::stoi(myString);
		myString.clear();
		Point2f pos{ Point2f((float)X,(float)Y) };
		m_Lavapools.push_back(new LavaPool(pos, m_ParticleManager));
	}
}

void Level::InitRopes()
{
	std::ifstream myFileStream("RopePositions.txt");
	if (!myFileStream.is_open())
	{
		Log("Positions of apples failed to load!");
		throw std::invalid_argument("Failed to load the textfile with the pickup positions");
	}
	int X, Y, ropeLength;
	std::string myString, line;


	//Initializing applepickups in the game
	while (std::getline(myFileStream, line))
	{
		std::stringstream stream(line);
		std::getline(stream, myString, ',');
		X = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ',');
		Y = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ' ');
		ropeLength = std::stoi(myString);
		myString.clear();
		Point2f pos{ Point2f((float)X,(float)Y) };
		m_Ropes.push_back(new Rope(pos, (float)ropeLength));
	}
}

void Level::InitFallingPlatforms()
{
	std::ifstream myFileStream("FallingPlatformsPositions.txt");
	if (!myFileStream.is_open())
	{
		Log("Positions of apples failed to load!");
		throw std::invalid_argument("Failed to load the textfile with the pickup positions");
	}
	int X, Y;
	std::string myString, line;


	//Initializing applepickups in the game
	while (std::getline(myFileStream, line))
	{
		std::stringstream stream(line);
		std::getline(stream, myString, ',');
		X = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ' ');
		Y = std::stoi(myString);
		myString.clear();
		Point2f pos{ Point2f((float)X,(float)Y) };
		m_FallingPlatforms.push_back(new FallingPlatform(pos));
	}
}

void Level::InitPlatforms()
{

	std::ifstream myFileStream("PlatformPositions.txt");
	if (!myFileStream.is_open())
	{
		Log("Positions of platforms failed to load!");
		throw std::invalid_argument("Failed to load the textfile with the pickup positions");
	}
	int X, Y, platformLength;
	std::string myString, line;


	//Initializing applepickups in the game
	while (std::getline(myFileStream, line))
	{
		std::stringstream stream(line);
		std::getline(stream, myString, ',');
		X = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ',');
		Y = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ' ');
		platformLength = std::stoi(myString);
		myString.clear();
		Point2f pos{ Point2f((float)X,(float)Y) };
		m_Platforms.push_back(new Platform(pos, (float)platformLength));
	}

}




#pragma region ENEMIES

void Level::InitEnemies()
{
	InitRangedEnemies();
	InitPotThrowerEnemies();
	InitGuards();
	InitCasketEnemies();
}


void Level::InitRangedEnemies()
{
	std::ifstream myFileStream("RangedEnemiesPositions.txt");
	if (!myFileStream.is_open())
	{
		Log("Positions of RangedEnemies failed to load!");
		throw std::invalid_argument("Failed to load the textfile with the pickup positions");
	}
	int X, Y;
	std::string myString, line;


	//Initializing RangedEnemies in the game
	while (std::getline(myFileStream, line))
	{
		std::stringstream stream(line);
		std::getline(stream, myString, ',');
		X = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ' ');
		Y = std::stoi(myString);
		myString.clear();
		Point2f pos{ Point2f((float)X,(float)Y) };
		m_AliveEnemies.push_back(new RangedEnemy(pos, m_AliveProjectiles, *m_LevelSoundManager,m_pTextureManager));
	}
}

void Level::InitPotThrowerEnemies()
{

	std::ifstream myFileStream("PotThrowersPositions.txt");
	if (!myFileStream.is_open())
	{
		Log("Positions of potThrowers failed to load!");
		throw std::invalid_argument("Failed to load the textfile with the pickup positions");
	}
	int X, Y;
	std::string myString, line;


	//Initializing PotThrowers in the game
	while (std::getline(myFileStream, line))
	{
		std::stringstream stream(line);
		std::getline(stream, myString, ',');
		X = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ' ');
		Y = std::stoi(myString);
		myString.clear();
		Point2f pos{ Point2f((float)X,(float)Y) };
		m_AliveEnemies.push_back(new PotThrower(pos, m_AliveProjectiles, *m_LevelSoundManager, m_pTextureManager));
	}


}

void Level::InitGuards()
{
	std::ifstream myFileStream("GuardPositions.txt");
	if (!myFileStream.is_open())
	{
		Log("Positions of Guards failed to load!");
		throw std::invalid_argument("Failed to load the textfile with the pickup positions");
	}
	int X, Y;
	std::string myString, line;


	//Initializing Guards in the game
	while (std::getline(myFileStream, line))
	{
		std::stringstream stream(line);
		std::getline(stream, myString, ',');
		X = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ' ');
		Y = std::stoi(myString);
		myString.clear();
		Point2f pos{ Point2f((float)X,(float)Y) };
		m_AliveEnemies.push_back(new Guard(pos, *m_LevelSoundManager, m_pTextureManager));
	}
}

void Level::InitCasketEnemies()
{
	std::ifstream myFileStream("CasketEnemies.txt");
	if (!myFileStream.is_open())
	{
		Log("Positions of Guards failed to load!");
		throw std::invalid_argument("Failed to load the textfile with the pickup positions");
	}
	int X, Y;
	std::string myString, line;


	//Initializing Guards in the game
	while (std::getline(myFileStream, line))
	{
		std::stringstream stream(line);
		std::getline(stream, myString, ',');
		X = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ' ');
		Y = std::stoi(myString);
		myString.clear();
		Point2f pos{ Point2f((float)X,(float)Y) };
		m_AliveEnemies.push_back(new CasketEnemy(pos, *m_LevelSoundManager, m_pTextureManager));
	}
}


#pragma endregion



void Level::InitPickUpPositions()
{
	InitApples();
	InitHealth();
}

void Level::InitApples()
{
	std::ifstream myFileStream("ApplePickUpsPositions.txt");
	if (!myFileStream.is_open())
	{
		Log("Positions of apples failed to load!");
		throw std::invalid_argument("Failed to load the textfile with the pickup positions");
	}
	int X, Y;
	std::string myString, line;

	//Initializing applepickups in the game
	while (std::getline(myFileStream, line))
	{
		std::stringstream stream(line);
		std::getline(stream, myString, ',');
		X = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ' ');
		Y = std::stoi(myString);
		myString.clear();
		Point2f pos{ Point2f((float)X,(float)Y) };
		m_PickUps.push_back(new ApplePickUp(pos));
	}
}

void Level::InitHealth()
{
	std::ifstream myFileStream("HealthPickUpPositions.txt");
	if (!myFileStream.is_open())
	{
		Log("Positions of apples failed to load!");
		throw std::invalid_argument("Failed to load the textfile with the pickup positions");
	}
	int X, Y;
	std::string myString, line;


	//Initializing applepickups in the game
	while (std::getline(myFileStream, line))
	{
		std::stringstream stream(line);
		std::getline(stream, myString, ',');
		X = std::stoi(myString);
		myString.clear();
		std::getline(stream, myString, ' ');
		Y = std::stoi(myString);
		myString.clear();
		Point2f pos{ Point2f((float)X,(float)Y) };
		m_PickUps.push_back(new HealthPickUp(pos));
	}

}

void Level::DebugDraw() const
{
	for (LavaPool* lavaPool : m_Lavapools)
	{
		utils::DrawPolygon(lavaPool->GetPlatformPoints());
	}

	for (Rope* rope : m_Ropes)
	{
		utils::DrawPolygon(rope->GetRopeCollisionPoints());
	}

	for (FallingPlatform* fallingPlatform : m_FallingPlatforms)
	{
		utils::DrawPolygon(fallingPlatform->GetPlatformPoints());
	}

	for (Platform* platform : m_Platforms)
	{
		utils::DrawPolygon(platform->GetPlatformPoints());
	}

	for (Enemy* enemy : m_AliveEnemies)
	{
		utils::DrawRect(enemy->GetHitBox());
	}

	for (Projectile* projectile : m_AliveProjectiles)
	{
		utils::DrawRect(projectile->GetHitBox());
	}

	for (PickUp* pickUp : m_PickUps)
	{
		utils::DrawRect(pickUp->GetHitBox());
	}
}

Player* Level::GetPlayer() const
{
	return m_Player;
}
