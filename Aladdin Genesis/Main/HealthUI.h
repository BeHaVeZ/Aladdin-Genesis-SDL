#pragma once
#include "UIBase.h"


class HealthUI final : public UIBase
{
public:
	HealthUI(const Point2f& pos,const TextureManager* textureManager);
	~HealthUI() = default;


	void Update(float elapsedSec) override;
	void Draw() const override;
	
	void SetHealth(int health);

private:
	int m_HealthAmount;
};