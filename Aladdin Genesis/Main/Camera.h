#pragma once

//----------------------------------------------------------------------------
//
//                                 Camera class
//
//----------------------------------------------------------------------------
class Camera final
{
public:
	//Constructor
	Camera(float width, float height);
	//Destructor
	~Camera();



	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Draw() const;
	void Update(const Rectf& target);

	Point2f GetPosition() const;


	Point2f GetCameraPos() const;
private:
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// Camera member variables
	//
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	float
		m_Width,
		m_Height;
	Rectf
		m_LevelBoundaries;
	Point2f
		m_CameraPos;

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	//
	// Camera member function declarations
	//
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	Point2f
		Track(const Rectf& target) const;
	void
		Clamp(Point2f& bottomLeftPos) const;

};