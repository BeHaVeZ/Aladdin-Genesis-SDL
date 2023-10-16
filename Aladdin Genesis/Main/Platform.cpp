#include "pch.h"
#include "Platform.h"

Platform::Platform(Point2f position, float platformLength) :
	m_PlatformPosition{ position },
	m_PlatformLength{ platformLength }
{
	m_PlatformPoints.push_back(Point2f(position.x, position.y));
	m_PlatformPoints.push_back(Point2f(position.x + platformLength, position.y));
}

Platform::~Platform()
{
}

std::vector<Point2f> Platform::GetPlatformPoints() const
{
	return m_PlatformPoints;
}