#pragma once
#include <vector>

//----------------------------------------------------------------------------
//
//                                 Falling Platform class
//
//----------------------------------------------------------------------------




class Texture;


class FallingPlatform
{
public:
	// ctor
	FallingPlatform(Point2f pos);
	
	// common methods
	void Draw() const;
	void Update(float elapsedSec);

	// dtor
	~FallingPlatform();

	// Getters | Setters
	std::vector<Point2f> GetPlatformPoints() const;

	void SetFalling();


private:

	//member variables
	Texture* m_pPlatformTexture;
	
	Point2f m_PlatformPosition;
	Point2f m_StartPos;

	std::vector<Point2f> m_PlatformPoints;

	bool m_IsFalling;

	float m_AccumulatedSeconds;

	float m_FallingSpeed;

	float m_VerticalBorderForReset;
};
