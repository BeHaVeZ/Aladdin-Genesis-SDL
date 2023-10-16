#include "pch.h"
#include "FallingPlatform.h"
#include <Texture.h>

FallingPlatform::FallingPlatform(Point2f pos) :
	m_pPlatformTexture{ new Texture("map/block.png") },
	m_PlatformPosition{ pos },
	m_StartPos{ pos },
	m_IsFalling{ false },
	m_AccumulatedSeconds{ 0 },
	m_FallingSpeed{350.f},
	m_VerticalBorderForReset{250.f}
{
	Point2f platformPointLeft{ Point2f(m_PlatformPosition.x,m_PlatformPosition.y + m_pPlatformTexture->GetHeight()) };
	Point2f platformPointRight{ Point2f(m_PlatformPosition.x + m_pPlatformTexture->GetWidth(),m_PlatformPosition.y + m_pPlatformTexture->GetHeight()) };
	m_PlatformPoints.push_back(platformPointLeft);
	m_PlatformPoints.push_back(platformPointRight);
}

void FallingPlatform::Draw() const
{
	m_pPlatformTexture->Draw(m_PlatformPosition);
}

void FallingPlatform::Update(float elapsedSec)
{
	if (m_IsFalling == true)
	{
		m_AccumulatedSeconds += elapsedSec;
		if (m_PlatformPosition.y < m_StartPos.y - m_VerticalBorderForReset)
		{
			m_PlatformPosition = m_StartPos;
			m_IsFalling = false;
			m_AccumulatedSeconds = 0;
			for (int i = 0; i < m_PlatformPoints.size(); i++)
			{
				m_PlatformPoints[i].y = m_StartPos.y + m_pPlatformTexture->GetHeight();
			}
		}
		else if (m_AccumulatedSeconds > .2f)
		{
			m_PlatformPosition.y -= m_FallingSpeed * elapsedSec;

			for (int i = 0; i < m_PlatformPoints.size(); i++)
			{
				m_PlatformPoints[i].y = m_PlatformPosition.y;
			}

		}
	}
}

FallingPlatform::~FallingPlatform()
{
	delete m_pPlatformTexture;
}

std::vector<Point2f> FallingPlatform::GetPlatformPoints() const
{
	return m_PlatformPoints;
}

void FallingPlatform::SetFalling()
{
	m_IsFalling = true;
}
