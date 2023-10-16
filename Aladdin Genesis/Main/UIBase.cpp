#include "pch.h"
#include "UIBase.h"
#include <Texture.h>


UIBase::UIBase(const Point2f& UIPos, const TextureManager* textureManager) :
	m_UIPos{ UIPos },
	m_SrcRect{ 0,0,0,0 },
	m_DestRect{ m_UIPos.x,m_UIPos.y,0,0 },
	m_pTextureManager{nullptr},
	m_FrameSpeed{},
	m_FrameWidth{}, m_FrameHeight{},
	m_AnimTime{},m_NrOfFrames{}
{
}
