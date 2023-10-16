#pragma once
#include <vector>
#include <Vector2f.h>

//----------------------------------------------------------------------------
//
//                                 Rope class
//
//----------------------------------------------------------------------------


class Rope final
{
public:
	Rope(const Point2f& ropeStartPos,const float m_RopeLength);
	~Rope();

	std::vector<Point2f> GetRopeCollisionPoints() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;

	bool IsColliding(const Rectf& actorShape) const;


private:
	std::vector<Point2f> m_RopeCollisionPoints;
	Point2f m_RopeStartPos;
	float m_RopeLength;
};