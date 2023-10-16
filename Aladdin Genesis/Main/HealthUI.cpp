#include "pch.h"
#include "HealthUI.h"
#include "UIBase.h"
#include <Texture.h>
#include "TextureManager.h"


HealthUI::HealthUI(const Point2f& pos, const TextureManager* textureManager) : UIBase(pos,textureManager)
{
	m_FrameWidth = 118.f;
	m_FrameSpeed = .2f;
	m_FrameHeight = 40.f;
	m_NrOfFrames = 4;
	m_AnimTime = 0;
	m_pTextureManager = textureManager;
	m_pTexture = textureManager->GetTextureReference(TextureManager::TextureKey::HealthUITxr);
	m_DestRect = Rectf(pos.x, pos.y, 250, 80);
	m_SrcRect = Rectf(0, 0, m_FrameWidth, m_FrameHeight);
	m_HealthAmount = 9;
}


void HealthUI::Update(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.left += m_FrameWidth;
		if (m_SrcRect.left >= m_pTexture->GetWidth())
		{
			m_SrcRect.left = 0;
		}
		m_AnimTime = 0;
	}

	switch (m_HealthAmount)
	{
	case 1: m_SrcRect.bottom = m_FrameHeight * 9;
		break;
	case 2: m_SrcRect.bottom = m_FrameHeight * 8;
		break;
	case 3: m_SrcRect.bottom = m_FrameHeight * 7;
		break;
	case 4: m_SrcRect.bottom = m_FrameHeight * 6;
		break;
	case 5: m_SrcRect.bottom = m_FrameHeight * 5;
		break;
	case 6: m_SrcRect.bottom = m_FrameHeight * 4;
		break;
	case 7: m_SrcRect.bottom = m_FrameHeight * 3;
		break;
	case 8:	m_SrcRect.bottom = m_FrameHeight * 2;
		break;
	case 9: m_SrcRect.bottom = m_FrameHeight * 1;
		break;
	}
}
void HealthUI::Draw() const
{
	m_pTexture->Draw(m_DestRect, m_SrcRect);
}

void HealthUI::SetHealth(int health)
{
	m_HealthAmount = health;
}
