#pragma once
#include <vector>

class Platform
{
public:
	Platform(Point2f position, float platformLength);
	~Platform();


	std::vector<Point2f> GetPlatformPoints() const;
private:
	Point2f 
		m_PlatformPosition;
	float
		m_PlatformLength;
	std::vector<Point2f> m_PlatformPoints;

};
