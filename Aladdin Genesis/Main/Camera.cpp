#include "pch.h"
#include "Camera.h"
#include <utils.h>

//Constructor
Camera::Camera(float width, float height) :
	m_Width{ 225 },
	m_Height{ 195 },
	m_LevelBoundaries{ 0,0,width,height },
	m_CameraPos{ 0,0 }
{

}
//Destructor
Camera::~Camera()
{

}

void Camera::Update(const Rectf& target)
{
	m_CameraPos = Track(target);
	Clamp(m_CameraPos);
}

Point2f Camera::GetPosition() const
{
	return m_CameraPos;
}

void Camera::Draw() const
{
	////for debug purposes this method is created uncomment this scope to draw where the camera will DRAW
	//utils::SetColor(Color4f(0, 0, 1, 1));
	//utils::DrawRect(m_CameraPos.x, m_CameraPos.y, m_Width, m_Height);
}

Point2f Camera::GetCameraPos() const
{
	return m_CameraPos;
}

//Sets the boundaries for the camera to stay inside the level
void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

//Tracks a target(player)
Point2f Camera::Track(const Rectf& target) const
{
	Point2f point{ target.left + target.width / 2 , target.bottom + target.height / 2 };
	point = Point2f{ point.x - m_Width / 2, point.y - m_Height / 2 };
	return point;
}

//Stays inside the level boundaries
void Camera::Clamp(Point2f& bottomLeftPos) const
{
	if (bottomLeftPos.x < m_LevelBoundaries.left)
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}
	else if (bottomLeftPos.x + m_Width > m_LevelBoundaries.left + m_LevelBoundaries.width)
	{
		bottomLeftPos.x = m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width;
	}
	if (bottomLeftPos.y < m_LevelBoundaries.left)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}
	else if (bottomLeftPos.y + m_Height > m_LevelBoundaries.bottom + m_LevelBoundaries.height)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height;
	}
}