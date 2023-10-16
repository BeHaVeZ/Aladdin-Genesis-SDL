#pragma once
#include "PickUp.h"
class Player;
class HealthPickUp final : public PickUp
{
public:
	HealthPickUp(const Point2f& pos);
	~HealthPickUp() = default;

	virtual const Rectf& GetHitBox() const override;

	virtual void Update(float elapsedSec) override;
	virtual bool IsHealthPickUp() const override;
};