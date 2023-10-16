#pragma once
#include "PickUp.h"


class ApplePickUp final : public PickUp
{
public:
	ApplePickUp(Point2f pos);
	const Rectf& GetHitBox() const override;
	virtual bool IsApplePickUp() const override;
};