#include "pch.h"
#include "HealthPickUp.h"
#include <Texture.h>

HealthPickUp::HealthPickUp(const Point2f& pos) : 
	PickUp(pos)
{
	m_AnimTime = 0;
	m_FrameSpeed = 0.1f;
	m_NrOfFrames = 8;
	m_PickUpTexture = new Texture("sprites/HealthPickUp.png");
	m_FrameWidth = m_PickUpTexture->GetWidth() / m_NrOfFrames;
	m_DestRect = Rectf(pos.x, pos.y, 20, 25);
	m_SrcRect = Rectf(0,m_PickUpTexture->GetHeight(),m_FrameWidth, m_PickUpTexture->GetHeight());
	m_HitBox = Rectf(pos.x, pos.y, m_FrameWidth, m_PickUpTexture->GetHeight());
}

const Rectf& HealthPickUp::GetHitBox() const
{
	return m_HitBox;
}

void HealthPickUp::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.left += m_FrameWidth;
		if (m_SrcRect.left >= m_PickUpTexture->GetWidth())
		{
			m_SrcRect.left = 0;
		}
		m_AnimTime = 0;
	}
}

bool HealthPickUp::IsHealthPickUp() const
{
	return true;
}