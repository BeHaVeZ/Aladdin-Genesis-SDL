#include "pch.h"
#include "PickUp.h"
#include <Texture.h>
#include <utils.h>

PickUp::PickUp(Point2f position) :
	m_NrOfFrames{ 0 },
	m_PickUpTexture{ NULL },
	m_Position{ position },
	//Animation
	m_AnimTime{ 0 }, m_FrameSpeed{ .5f },m_FrameWidth{10.f}
{
}

void PickUp::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;
}

void PickUp::Draw() const
{
	m_PickUpTexture->Draw(m_DestRect,m_SrcRect);
}

PickUp::~PickUp()
{
	delete m_PickUpTexture;
}

bool PickUp::IsHit(const Rectf& actorShape) const
{
	if (utils::IsOverlapping(actorShape, m_HitBox))
	{
		return true;
	}
	return false;
}

bool PickUp::IsHealthPickUp() const
{
	return false;
}

bool PickUp::IsApplePickUp() const
{
	return false;
}
