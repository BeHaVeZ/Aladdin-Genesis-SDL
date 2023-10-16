#include "pch.h"
#include "ApplePickUp.h"
#include <Texture.h>


ApplePickUp::ApplePickUp(Point2f pos) : PickUp(pos)
{
	m_PickUpTexture = new Texture("sprites/PickUp_Apple.png");
	m_DestRect = Rectf(pos.x, pos.y, m_PickUpTexture->GetWidth(), m_PickUpTexture->GetHeight());
	m_HitBox = m_DestRect;
}
const Rectf& ApplePickUp::GetHitBox() const
{
	return m_DestRect;
}

bool ApplePickUp::IsApplePickUp() const
{
	return true;
}