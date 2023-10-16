#include "pch.h"
#include "Projectile.h"
#include "TextureManager.h"
#include <Texture.h>

Projectile::Projectile(Point2f& startPos,const TextureManager* textureManager) : 
	m_Position{startPos},
	m_pProjectileTexture{nullptr},
	m_TextureManagerReference{textureManager}
{	
}

const bool Projectile::IsAppleProjectile() const
{
	return false;
}

const bool Projectile::IsPotProjectile() const
{
	return false;
}
