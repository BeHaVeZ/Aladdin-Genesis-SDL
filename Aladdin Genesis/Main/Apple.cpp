#include "pch.h"
#include "Apple.h"
#include <Texture.h>
#include <utils.h>
#include "TextureManager.h"


Apple::Apple(Point2f spawnPos, bool isFlyingLeft,const TextureManager* textureManager) : Projectile(spawnPos,textureManager),
//Physics
m_Velocity_X{ 350 }, m_Velocity_Y{ 100 },
//Rects
m_DestRect{ spawnPos.x,spawnPos.y,20,17 }, m_SrcRect{ 0,0,30,27 },
//Logical operator
m_IsFlyingLeft{ isFlyingLeft },
m_Angle{ 0 }, m_RotationSpeed{ 1 }
{
	m_pProjectileTexture = textureManager->GetTextureReference(TextureManager::TextureKey::AppleProjectileTxr);
}


void Apple::Update(float elapsedSec)
{

	if (m_IsFlyingLeft)
	{
		m_DestRect.left -= m_Velocity_X * elapsedSec;
	}
	else {
		m_DestRect.left += m_Velocity_X * elapsedSec;
	}

	m_DestRect.bottom -= m_Velocity_Y * elapsedSec;
	m_Velocity_Y += .2f;
	m_Angle += m_RotationSpeed;

}

void Apple::Draw() const
{
	//roteert de appel wanneer deze in de lucht is
	glPushMatrix();
	{
		glTranslatef(m_DestRect.left, m_DestRect.bottom, 0);
		glTranslatef(m_DestRect.width / 2, m_DestRect.height / 2, 0);
		glRotatef(m_Angle, 0, 0, 1);
		glTranslatef(-m_DestRect.left, -m_DestRect.bottom, 0);
		glTranslatef(-m_DestRect.width / 2, -m_DestRect.height / 2, 0);
		m_pProjectileTexture->Draw(m_DestRect, m_SrcRect);
		//utils::DrawRect(m_DestRect);
	}
	glPopMatrix();
}

const float Apple::GetWidth() const
{
	return m_pProjectileTexture->GetWidth();
}

const float Apple::GetHeight() const
{
	return m_pProjectileTexture->GetHeight();
}

const bool Apple::CanHurtEnemy() const
{
	return true;
}

Rectf Apple::GetRect() const
{
	return m_DestRect;
}

const Rectf& Apple::GetHitBox() const
{
	return m_DestRect;
}

const bool Apple::IsAppleProjectile() const
{
	return true;
}
