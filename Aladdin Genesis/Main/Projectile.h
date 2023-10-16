#pragma once

class Texture;
class TextureManager;

class Projectile
{
public:

	Projectile(Point2f& startPos,const TextureManager* textureManager);
	virtual ~Projectile() = default;


	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;
	virtual const Rectf& GetHitBox() const = 0;

	virtual const bool CanHurtEnemy() const = 0;

	virtual const bool IsAppleProjectile() const;
	virtual const bool IsPotProjectile() const;


protected:
	Point2f m_Position;
	const TextureManager* m_TextureManagerReference;
	const Texture* m_pProjectileTexture;
};