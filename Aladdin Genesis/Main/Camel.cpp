#include "pch.h"
#include "Camel.h"
#include <Texture.h>
#include <utils.h>
#include "Spit.h"
#include "Projectile.h"
#include "TextureManager.h"


Camel::Camel(const Point2f& pos,std::vector<Projectile*>& projectiles,const TextureManager& textureManager) :
	//Texture
	m_CamelTexture{ textureManager.GetTextureReference(TextureManager::TextureKey::CamelTxr)},
	//Rects
	m_DestRect{ pos.x,pos.y,150,50 }, m_SrcRect{ 0,0,177,75 },
	//CamelStates
	m_CurrentCamelState{ CamelStates::idle },
	//Animation
	m_FrameSpeed{.05f},m_AnimTime{0},m_NrFrames{10},m_FrameWidth{177},
	//Logical Operators
	m_HasShot{false},
	m_LevelProjectiles{projectiles},
	m_TextureManager{textureManager}

{
	m_CamelCollisionPoints.push_back(Point2f(m_DestRect.left, m_DestRect.bottom + m_DestRect.height/2));
	m_CamelCollisionPoints.push_back(Point2f(m_DestRect.left + m_DestRect.width/3, m_DestRect.bottom + m_DestRect.height/2));
}

void Camel::Draw() const 
{
	m_CamelTexture->Draw(m_DestRect,m_SrcRect);
}

void Camel::Update(float elapsedSec) 
{
	m_AnimTime += elapsedSec;
	switch (m_CurrentCamelState)
	{
	case Camel::CamelStates::idle:
		HandleIdleAnimation();
		break;
	case Camel::CamelStates::shooting:
		HandleShootingAnimation();
		break;
	}
}

void Camel::Shoot()
{
	m_CurrentCamelState = CamelStates::shooting;
}
std::vector<Point2f> Camel::GetCamelCollisionPoints() const
{
	return m_CamelCollisionPoints;
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Animation handling
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


void Camel::HandleIdleAnimation()
{
	m_SrcRect.left = 0;
}

void Camel::HandleShootingAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.left += m_FrameWidth;

		if (m_SrcRect.left >= m_FrameWidth * m_NrFrames/2 and m_HasShot == false)
		{
			m_LevelProjectiles.push_back(new Spit(Point2f(m_DestRect.left + m_DestRect.width - m_DestRect.width * .3f, m_DestRect.bottom + m_DestRect.height * .3f), true,&m_TextureManager));
			m_HasShot = true ;
		}

		if (m_SrcRect.left >= m_FrameWidth * m_NrFrames)
		{
			m_SrcRect.left = 0;
			m_CurrentCamelState = CamelStates::idle;
			m_HasShot = false;
		}
		m_AnimTime = 0;
	}
}
