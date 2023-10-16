#pragma once

class Texture;
class TextureManager;

class UIBase
{
public:
	UIBase(const Point2f& UIPos,const TextureManager* textureManager);
	~UIBase() = default;


	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;



protected:

	Point2f m_UIPos;
	const Texture* m_pTexture;
	const TextureManager* m_pTextureManager;
	Rectf m_SrcRect, m_DestRect;

	float		m_FrameWidth;
	float		m_FrameHeight;
	float		m_AnimTime;
	float		m_FrameSpeed;
	int			m_NrOfFrames;

};

