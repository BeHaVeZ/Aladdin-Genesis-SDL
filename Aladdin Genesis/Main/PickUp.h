#pragma once

class Texture;


class PickUp
{
public:

	PickUp(Point2f position);
	virtual void Update(float elapsedSec);
	virtual void Draw() const;

	~PickUp();

	//TODO: Rule of 5


	virtual const Rectf& GetHitBox() const = 0;
	virtual bool IsHit(const Rectf& actorShape) const;

	virtual bool IsHealthPickUp() const;
	virtual bool IsApplePickUp() const;


protected:

	Texture* m_PickUpTexture;
	Point2f m_Position;


	Rectf m_DestRect;
	Rectf m_SrcRect;
	Rectf m_HitBox;

	//Animation
	int m_NrOfFrames;
	float
		m_FrameWidth,
		m_AnimTime,
		m_FrameSpeed;
};