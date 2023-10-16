#pragma once

//----------------------------------------------------------------------------
//
//                                 Enemy Base class
//
//----------------------------------------------------------------------------

class Texture;
class SoundManager;
class TextureManager;



class Enemy
{
public:

	Enemy(Point2f& startPos,const SoundManager& sm,const TextureManager* textureManager);
	Enemy(Enemy&&) = default;
	Enemy& operator=(const Enemy&) = default;
	Enemy& operator=(const Enemy&&) = delete;
	virtual ~Enemy() = default;


	virtual void Update(float elapsedSec,const Rectf& actorShape) = 0;
	virtual void Draw() const = 0;
	virtual const Rectf& GetHitBox() const = 0;
	virtual const bool IsPotThrower() const;
	virtual const bool IsGuard() const;
	virtual const bool IsCasketEnemy() const;
	virtual bool CanBeDamaged();

	virtual void ReceiveDamage();
	virtual int GetCurrentHealth() const = 0;

protected:
	Point2f m_Position;

	const SoundManager& m_SoundManagerRef;
	const TextureManager* m_TextureManagerRef;
	const Texture* m_pEnemyTexture;

	int m_Health;
	bool m_CanBeDamaged;
	float m_TimeBetweenDamages, m_DamageTimer;

	int			m_NrOfFrames;
	float		m_FrameWidth;
	float		m_FrameHeight;
	float		m_AnimTime;
	float		m_FrameSpeed;
	float		m_MarginBetweenFrames;
};