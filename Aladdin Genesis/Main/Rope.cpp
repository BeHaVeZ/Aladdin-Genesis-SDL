#include "pch.h"
#include "Rope.h"
#include <utils.h>

Rope::Rope(const Point2f& ropeStartPos, const float ropeLength):
	m_RopeStartPos{ropeStartPos},
	m_RopeLength{ ropeLength }
{
	m_RopeCollisionPoints.push_back(Point2f(ropeStartPos));
	m_RopeCollisionPoints.push_back(Point2f(ropeStartPos.x, ropeStartPos.y - ropeLength));
}

Rope::~Rope()
{
}

std::vector<Point2f> Rope::GetRopeCollisionPoints() const
{
	return m_RopeCollisionPoints;
}

void Rope::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	utils::HitInfo hit{  };
	if (utils::Raycast(m_RopeCollisionPoints,
		Point2f(actorShape.left + actorShape.width * .40f, actorShape.bottom + actorShape.height * .6f - 1),
		Point2f(actorShape.left + actorShape.width - actorShape.width * .40f, actorShape.bottom + actorShape.height * .6f - 1), hit))
	{
		actorShape.left = hit.intersectPoint.x - actorShape.width / 2;
		actorVelocity.y = 0;
	}
}

bool Rope::IsColliding(const Rectf& actorShape) const
{
	utils::HitInfo hit;
	if (utils::Raycast(m_RopeCollisionPoints,
		Point2f(actorShape.left + actorShape.width * .40f, actorShape.bottom + actorShape.height / 2),
		Point2f(actorShape.left + actorShape.width - actorShape.width * .40f, actorShape.bottom + actorShape.height / 2), hit))
	{
		return true;
	}
	return false;
}