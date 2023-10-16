#include "pch.h"
#include <utils.h>
#include "Player.h"
#include <Texture.h>
#include "Level.h"
#include "Projectile.h"
#include "Apple.h"
#include "SoundManager.h"
#include "TextureManager.h"

//Constructor
Player::Player(const Level& level,const TextureManager& textureManager) :
	//Player variables
	m_MaxPlayerHealth{9},
	m_PlayerHealth{ 9 }, //9
	//Bools
	m_IsWalkingLeft{ false }, m_IsAttacking{ false }, m_IsThrowing{ false }, m_IsClimbing{ false }, m_IsBeingDamaged{ false }, m_CanBeDamaged{ true },
	m_TimeBetweenDamage{ 2.f }, m_Timer{ 0 },
	//Rects
	m_DestRect{ 55,55,45, 65 },
	m_SrcRect{ 0,90,65 ,90 },
	m_HitBox{ m_DestRect.left + m_DestRect.width * .3f,m_DestRect.bottom,m_DestRect.width * .3f,m_DestRect.height * .6f },
	m_AttackZone{ 0,0,0,0 }, m_AttackRange{ 3.f },
	//States
	m_CurrentActionState{ ActionState::idle },
	m_PreviousActionState{ ActionState::idle },
	//Animation
	m_AnimTime{ 0 }, m_FrameSpeed{ .09f }, m_TotalTimeIdle{ 0 }, m_TimeBeforeLongIdle{ 5.f },
	m_NumberOfFrames{ 0 }, m_MarginBetweenFrames{ 65 },
	m_HorizontalSpeed{ 120.f }, m_JumpSpeed{ 340.f }, m_ClimbingSpeed{ 100.f },
	m_Acceleration{ Vector2f(0,-981.f) }, m_Velocity{ Vector2f(0,0) },
	//Apples
	m_Ammo{ 10 }, m_AppleWidth{ 20 }, m_AppleHeight{ 17 },
	//Climbing
	m_IdleFramePos{ 0 },
	m_Level{ &level },
	m_pTextureManager{&textureManager},
	m_AvatarSpritePtr{textureManager.GetTextureReference(TextureManager::TextureKey::playerTxr)}
{
}
//Update
void Player::Update(float elapsedSec, const Level& level)
{
	m_PreviousActionState = m_CurrentActionState;
	m_AnimTime += elapsedSec;

	//Hier wordt gecheckt of de speler al gedamaged werdt zodat hij een stuk tijd heeft om te recoveren
	if (m_CanBeDamaged == false)
	{
		m_Timer += elapsedSec;
		if (m_Timer >= m_TimeBetweenDamage)
		{
			m_CanBeDamaged = true;
			m_Timer = 0;
		}
	}
	//Hier Wordt gecheckt of de speler attackt als die attackt moet er een zone zijn om enemies later te damagen
	if (m_IsAttacking)
	{
		if (m_IsWalkingLeft)
		{
			m_AttackZone = Rectf(m_DestRect.left, m_DestRect.bottom, m_AttackRange, m_HitBox.height);
		}
		else
		{
			m_AttackZone = Rectf(m_DestRect.left + m_DestRect.width - m_AttackRange, m_DestRect.bottom, m_AttackRange, m_HitBox.height);
		}
	}
	else
	{
		m_AttackZone = Rectf(0, 0, 0, 0);
	}


	HandleMovement(level, elapsedSec);
	CheckStateChange();
	HandleAnimations();
}
//Draw
void Player::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_DestRect.left, m_DestRect.bottom, 0);
		if (m_IsWalkingLeft == true)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_DestRect.width, 0, 0);
		}
	}
	m_AvatarSpritePtr->Draw(Rectf(0, 0, m_DestRect.width, m_DestRect.height), m_SrcRect);
	glPopMatrix();

	#pragma region Debug // DrawCollisionBox
	//utils::DrawRect(m_DestRect);
	//utils::DrawRect(m_HitBox);
	#pragma endregion
}
//Returns the adress of players destRect
Rectf* Player::GetShape()
{
	return &m_DestRect;
}
Vector2f* Player::GetVelocity()
{
	return &m_Velocity;
}
const Player::ActionState Player::GetCurrentState() const
{
	return m_CurrentActionState;
}
const Rectf* Player::GetHitBox() const
{
	return &m_HitBox;
}
const Rectf* Player::GetAttackBox() const
{
	return &m_AttackZone;
}
int Player::GetPlayerHealth() const
{
	return m_PlayerHealth;
}
//Returns if the player is climbing or not
bool Player::IsClimbing() const
{
	return m_IsClimbing;
}
bool Player::CanBeDamaged() const
{
	return m_CanBeDamaged;
}
void Player::ResetPosition()
{
	//m_DestRect.left = 1200;
	//m_DestRect.bottom = 55;
	m_DestRect.left = 3151;
	m_DestRect.bottom = 366;
}
void Player::SetClimbingState(bool state)
{
	m_IsClimbing = state;
	m_Velocity.x = 0;
	m_Velocity.y = 0;
}
void Player::SetCurrentActionState(ActionState state)
{
	m_PreviousActionState = m_CurrentActionState;
	m_CurrentActionState = state;
}
void Player::SetVelocity(Vector2f velocity)
{
	m_Velocity = velocity;
}
void Player::AddHealth()
{
	if (m_PlayerHealth < m_MaxPlayerHealth)
	{
		++m_PlayerHealth;
	}
}
//Checks if the state has changed from the previous one
void Player::CheckStateChange()
{
	if (m_PreviousActionState != m_CurrentActionState and m_IsClimbing == false)
	{
		m_SrcRect.left = 0;
	}
}


#pragma region MovementHandling
void Player::HandleMovement(const Level& level, float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (m_IsClimbing == false)
	{
		if (pStates[SDL_SCANCODE_RIGHT])
		{
			m_TotalTimeIdle = 0;
			m_IsWalkingLeft = false;
			if (level.IsAgainstWall(m_DestRect) and level.IsOnGround(m_DestRect))
			{
				m_CurrentActionState = ActionState::pushing;
				m_Velocity.x = m_HorizontalSpeed;
			}
			else {
				m_CurrentActionState = ActionState::walking;
				m_Velocity.x = m_HorizontalSpeed;
			}
		}
		else if (pStates[SDL_SCANCODE_LEFT])
		{
			m_TotalTimeIdle = 0;
			m_IsWalkingLeft = true;
			if (level.IsAgainstWall(m_DestRect) and level.IsOnGround(m_DestRect))
			{
				m_CurrentActionState = ActionState::pushing;
				m_Velocity.x = -m_HorizontalSpeed;
			}
			else {
				m_CurrentActionState = ActionState::walking;
				m_Velocity.x = -m_HorizontalSpeed;
			}
		}
		else if (pStates[SDL_SCANCODE_DOWN] and m_Velocity.x == 0) {
			m_TotalTimeIdle = 0;
			m_CurrentActionState = ActionState::crouching;
		}
		else {
			m_Velocity.x = 0;
			if (m_TotalTimeIdle > m_TimeBeforeLongIdle)
			{
				m_CurrentActionState = ActionState::longidle;
			}
			else
				m_CurrentActionState = ActionState::idle;
		}
		if (level.IsOnGround(m_DestRect)) //jumping is only allowed when on ground
		{
			if (pStates[SDL_SCANCODE_Z])
			{
				Jump();
			}
		}
		else
		{
			m_CurrentActionState = ActionState::jumping;
		}
	}
	else if (m_IsClimbing == true)
	{
		if (pStates[SDL_SCANCODE_Z])
		{
			m_IsClimbing = false;
			Jump();
		}
		else if (pStates[SDL_SCANCODE_DOWN])
		{
			m_TotalTimeIdle = 0;
			m_CurrentActionState = ActionState::climbingdown;
			m_Velocity.y = -m_ClimbingSpeed;
		}
		else if (pStates[SDL_SCANCODE_UP]) {
			m_TotalTimeIdle = 0;
			m_CurrentActionState = ActionState::climbingup;
			m_Velocity.y = m_ClimbingSpeed;
		}
		else {
			m_Velocity.y = 0;
			m_CurrentActionState = ActionState::climbingidle;
		}
	}
	if (m_CurrentActionState == ActionState::crouching)
	{
		m_HitBox = Rectf(m_DestRect.left + m_DestRect.width * .3f, m_DestRect.bottom, m_DestRect.width * .3f, m_DestRect.height * .3f);
	}
	else {
		m_HitBox = Rectf(m_DestRect.left + m_DestRect.width * .3f, m_DestRect.bottom, m_DestRect.width * .3f, m_DestRect.height * .6f);
	}

	m_TotalTimeIdle += elapsedSec;
	m_DestRect.left += m_Velocity.x * elapsedSec;
	m_DestRect.bottom += m_Velocity.y * elapsedSec;
	m_Velocity += m_Acceleration * elapsedSec;
}

void Player::Jump()
{
	m_CurrentActionState = ActionState::jumping;
	m_TotalTimeIdle = 0;
	m_Velocity.y = m_JumpSpeed;
}
#pragma endregion

#pragma region AnimationHandling
void Player::HandleAnimations()
{
	if (m_IsBeingDamaged)
	{
		HandleDamagedAnimation();
	}
	//checking if the player is attacking to stay consistent with the correct animation
	else if (m_IsThrowing)
	{
		switch (m_CurrentActionState)
		{
		case Player::ActionState::idle:
			HandleIdleThrowAnimation();
			break;
		case Player::ActionState::longidle:
			HandleIdleThrowAnimation();
			break;
		case Player::ActionState::walking:
			HandleRunningThrowAnimation();
			break;
		case Player::ActionState::jumping:
			HandleJumpThrowAnimation();
			break;
		case Player::ActionState::crouching:
			HandleCrouchThrowAnimation();
			break;
		case Player::ActionState::pushing:
			HandleIdleThrowAnimation();
			break;
		}
	}
	//checking if the player is attacking to stay consistent with the correct animation
	else if (m_IsAttacking)
	{
		switch (m_CurrentActionState)
		{
		case Player::ActionState::idle:
			HandleIdleAttackAnimation();
			break;
		case Player::ActionState::longidle:
			HandleIdleAttackAnimation();
			break;
		case Player::ActionState::walking:
			HandleRunningAttackAnimation();
			break;
		case Player::ActionState::jumping:
			HandleJumpingAttackAnimation();
			break;
		case Player::ActionState::crouching:
			HandleCrouchingAttackAnimation();
			break;
		case Player::ActionState::pushing:
			HandleIdleAttackAnimation();
			break;
		}
	}
	else {
		switch (m_CurrentActionState)
		{
		case Player::ActionState::idle:
			HandleIdleAnimation();
			break;
		case Player::ActionState::longidle:
			HandleLongIdleAnimation();
			break;
		case Player::ActionState::walking:
			HandleRunningAnimation();
			break;
		case Player::ActionState::jumping:
			HandleJumpingAnimation();
			break;
		case Player::ActionState::crouching:
			HandleCrouchingAnimation();
			break;
		case Player::ActionState::pushing:
			HandlePushingAnimation();
		case Player::ActionState::climbingup:
			HandleClimbingUpAnimation();
			break;
		case Player::ActionState::climbingdown:
			HandleClimbingDownAnimation();
			break;
		case Player::ActionState::climbingidle:
			HandleIdleClimbingAnimation();
			break;
		}
	}
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// BASE Animation handling
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


//IDLE
void Player::HandleIdleAnimation()
{
	//checks the frame from the previous one
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = 90;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 17)
		{
			m_SrcRect.left = 0;
		}
		m_AnimTime = 0;

	}
}
void Player::HandleLongIdleAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = 90 * 2;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= m_AvatarSpritePtr->GetWidth())
		{
			m_SrcRect.left = 65 * 5;
		}
		m_AnimTime = 0;
	}
}
//RUNNING
void Player::HandleRunningAnimation()
{
	if (m_AnimTime >= m_FrameSpeed * .6f) //.7f default value is the speed of the animation (the less the faster)
	{
		m_SrcRect.bottom = 90 * 15;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 13)
		{
			m_SrcRect.left = 65 * 4;
		}
		m_AnimTime = 0;
	}
}
//JUMPING
void Player::HandleJumpingAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		if (m_Velocity.x > 0 or m_Velocity.x < 0)
		{
			m_SrcRect.bottom = 90 * 9;
			m_SrcRect.left += m_MarginBetweenFrames;
			if (m_SrcRect.left >= 65 * 6)
			{
				m_SrcRect.left = 65 * 4;
			}
			m_AnimTime = 0;
		}
		else {
			m_SrcRect.bottom = 90 * 10;
			m_SrcRect.left += m_MarginBetweenFrames;
			if (m_SrcRect.left >= 65 * 10)
			{
				m_SrcRect.left = 65 * 8;
			}
			m_AnimTime = 0;
		}
	}
}
//CROUCHING
void Player::HandleCrouchingAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = 90 * 5;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 4)
		{
			m_SrcRect.left = 65 * 3;
		}
		m_AnimTime = 0;
	}
}
//PUSHING
void Player::HandlePushingAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = 90 * 23;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 9)
		{
			m_SrcRect.left = 65 * 1;
		}
		m_AnimTime = 0;
	}
}
//CLIMBING
void Player::HandleClimbingUpAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = 90 * 17;
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left > 65 * 9)
		{
			m_SrcRect.left = 0;
		}
		m_AnimTime = 0;
	}
	m_IdleFramePos = m_SrcRect.left;
}

void Player::HandleClimbingDownAnimation()
{
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.bottom = 90 * 17;
		m_SrcRect.left -= m_MarginBetweenFrames;
		if (m_SrcRect.left < 0)
		{
			m_SrcRect.left = m_MarginBetweenFrames * 9;
		}
		m_AnimTime = 0;
	}
	m_IdleFramePos = m_SrcRect.left;
}

//CLIMBING_IDLE
void Player::HandleIdleClimbingAnimation()
{
	m_SrcRect.bottom = 90 * 17;
	m_SrcRect.left = m_IdleFramePos;
}


//DAMAGED_ANIMATION

void Player::Damage()
{
	if (m_CanBeDamaged == true)
	{
		m_Level->GetLevelSoundManager()->PlaySoundEffect(SoundManager::SoundEffects::PlayerDamaged);
		--m_PlayerHealth;
		m_IsBeingDamaged = true;
		m_SrcRect.left = 0;
		m_CanBeDamaged = false;
		m_TotalTimeIdle = 0;
	}
}


void Player::HandleDamagedAnimation()
{
	m_SrcRect.bottom = 90 * 26;
	if (m_AnimTime >= m_FrameSpeed * .7f)
	{
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 6)
		{
			m_SrcRect.left = 0;
			m_IsBeingDamaged = false;
		}
		m_AnimTime = 0;
	}
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ATTACK Animation handling
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//ATTACK_IDLE
void Player::HandleIdleAttackAnimation()
{
	m_SrcRect.bottom = 90 * 4;
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 4)
		{
			m_SrcRect.left = 0;
			m_IsAttacking = false;
		}
		m_AnimTime = 0;
	}
}
//ATTACK_RUNNING
void Player::HandleRunningAttackAnimation()
{
	m_SrcRect.bottom = 90 * 27;
	if (m_AnimTime >= m_FrameSpeed * .7f)
	{
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 4)
		{
			m_SrcRect.left = 0;
			m_IsAttacking = false;
		}
		m_AnimTime = 0;
	}
}
//ATTACK_CROUCHING
void Player::HandleCrouchingAttackAnimation()
{
	m_SrcRect.bottom = 90 * 8;
	if (m_AnimTime >= m_FrameSpeed * .7f)
	{
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 7)
		{
			m_SrcRect.left = 0;
			m_IsAttacking = false;
		}
		m_AnimTime = 0;
	}
}
//ATTACK_JUMPING
void Player::HandleJumpingAttackAnimation()
{
	m_SrcRect.bottom = 90 * 13;
	if (m_AnimTime >= m_FrameSpeed * .7f)
	{
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 5)
		{
			m_SrcRect.left = 0;
			m_IsAttacking = false;
		}
		m_AnimTime = 0;
	}
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// THROW Animation handling
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//IDLE_THROW
void Player::HandleIdleThrowAnimation()
{
	m_SrcRect.bottom = 90 * 3;
	if (m_AnimTime >= m_FrameSpeed)
	{
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 5)
		{
			m_SrcRect.left = 0;
			m_IsThrowing = false;
		}
		m_AnimTime = 0;
	}
}
//RUNNING_THROW
void Player::HandleRunningThrowAnimation()
{
	m_SrcRect.bottom = 90 * 28;
	if (m_AnimTime >= m_FrameSpeed * .7f)
	{
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 4)
		{
			m_SrcRect.left = 0;
			m_IsThrowing = false;
		}
		m_AnimTime = 0;
	}
}
//CROUCH_THROW
void Player::HandleCrouchThrowAnimation()
{
	m_SrcRect.bottom = 90 * 7;
	if (m_AnimTime >= m_FrameSpeed * .8f)
	{
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 6)
		{
			m_SrcRect.left = 0;
			m_IsThrowing = false;
		}
		m_AnimTime = 0;
	}
}
//JUMPING_THROW
void Player::HandleJumpThrowAnimation()
{
	m_SrcRect.bottom = 90 * 12;
	if (m_AnimTime >= m_FrameSpeed * .9f)
	{
		m_SrcRect.left += m_MarginBetweenFrames;
		if (m_SrcRect.left >= 65 * 4)
		{
			m_SrcRect.left = 0;
			m_IsThrowing = false;
		}
		m_AnimTime = 0;
	}
}

#pragma endregion

//ATTACKING
void Player::Attack()
{
	if (m_IsAttacking == false)
	{
		m_SrcRect.left = 0;
		m_IsAttacking = true;
		if (m_CurrentActionState != Player::ActionState::crouching)
		{
			m_Level->GetLevelSoundManager()->PlaySoundEffect(SoundManager::SoundEffects::HighSword);
		}
		else {
			m_Level->GetLevelSoundManager()->PlaySoundEffect(SoundManager::SoundEffects::LowSword);
		}
	}
}
//THROWING
void Player::Throw(std::vector<Projectile*>* projectiles)
{
	if (m_IsThrowing == false and m_Ammo > 0)
	{
		m_Level->GetLevelSoundManager()->PlaySoundEffect(SoundManager::SoundEffects::Throw);
		m_SrcRect.left = 0;
		m_IsThrowing = true;
		if (m_IsWalkingLeft)
		{
			projectiles->push_back(new Apple(Point2f(m_DestRect.left + m_DestRect.width / 2 - m_AppleWidth / 2, m_DestRect.bottom + m_DestRect.height / 2 - m_AppleHeight / 2), 1, m_pTextureManager));
		}
		else {
			projectiles->push_back(new Apple(Point2f(m_DestRect.left + m_DestRect.width / 2 - m_AppleWidth / 2, m_DestRect.bottom + m_DestRect.height / 2 - m_AppleHeight / 2), 0, m_pTextureManager));
		}
	}
}