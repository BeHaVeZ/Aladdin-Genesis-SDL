#pragma once
#include <Vector2f.h>

//class forwards
class Texture;
class Level;
class Apple;
class Projectile;
class SoundManager;
class TextureManager;

//----------------------------------------------------------------------------
//
//                                 Player class
//
//----------------------------------------------------------------------------
class Player final
{
public:

	//Constructor
	Player(const Level& level, const TextureManager& textureManager);
	~Player() = default;

	//Basic functions
	void Update(float elapsedSec, const Level& level);
	void Draw() const;

	//All the possible states
	enum class ActionState {
		//base states
		idle,
		longidle,
		walking,
		jumping,
		crouching,
		pushing,
		climbingidle,
		climbingup,
		climbingdown,
		damaged
	};

	//Getters
	Rectf* GetShape();
	Vector2f* GetVelocity();
	const ActionState GetCurrentState() const;
	const Rectf* GetHitBox() const;
	const Rectf* GetAttackBox() const;
	int GetPlayerHealth() const;

	bool IsClimbing() const;
	bool CanBeDamaged() const;


	//Setters
	void ResetPosition();
	void SetClimbingState(bool state);
	void SetCurrentActionState(ActionState state);
	void SetVelocity(Vector2f velocity);
	
	void AddHealth();

private:
	//Player variables
	void CheckStateChange();
	const int m_MaxPlayerHealth;
	int m_PlayerHealth;

	const Level* m_Level;


	//State of the player
	ActionState
		m_CurrentActionState,
		m_PreviousActionState;

	//Logical Operators
	bool
		m_IsWalkingLeft,
		m_IsClimbing,
		m_IsThrowing, 
		m_IsAttacking, 
		m_IsBeingDamaged,
		m_CanBeDamaged;

	//Logical floats
	float m_TimeBetweenDamage, m_Timer;

	//Pointer to texture on heap on init
	const Texture* m_AvatarSpritePtr;
	const TextureManager* m_pTextureManager;

	//Rects
	Rectf
		m_DestRect,
		m_SrcRect,
		m_HitBox,
		m_AttackZone;

	float
		m_MarginBetweenFrames;

	int
		m_NumberOfFrames;


	//Apples
	int
		m_Ammo;
	float
		m_AppleWidth, m_AppleHeight;

	//Climbing
	float m_IdleFramePos;

	#pragma region Movement
public:
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Movement Functions
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	void HandleMovement(const Level& level, float elapsedSec);

	void Jump();


private:
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Movement variables
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	const float
		m_HorizontalSpeed,
		m_ClimbingSpeed,
		m_JumpSpeed;
	Vector2f
		m_Velocity,
		m_Acceleration;

	#pragma endregion

	#pragma region Attacking
public:
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Attack Function
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	void Attack();

	void Throw(std::vector<Projectile*>* projectiles);

	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Attack Variables
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	private:
	float m_AttackRange;
	public:
	void Damage();


	#pragma endregion



	#pragma region Animations
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Animation functions
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
private:
	void HandleAnimations();
	///BASE
	//Running
	void HandleRunningAnimation();
	//Idle
	void
		HandleIdleAnimation(),
		HandleLongIdleAnimation();
	//Jumping
	void HandleJumpingAnimation();
	//Crouching
	void HandleCrouchingAnimation();
	//Pushing
	void
		HandlePushingAnimation();
	//Climbing
	void
		HandleClimbingUpAnimation(),
		HandleClimbingDownAnimation(),
		HandleIdleClimbingAnimation();
	//Damaged
	void
		HandleDamagedAnimation();

	///ATTACKING
	//AttackIdle
	void HandleIdleAttackAnimation();
	//AttackRunning
	void HandleRunningAttackAnimation();
	//AttackCrouching
	void HandleCrouchingAttackAnimation();
	//AttackJumping
	void HandleJumpingAttackAnimation();
	///THROWING
	//ThrowIdle
	void HandleIdleThrowAnimation();
	//ThrowRunning
	void HandleRunningThrowAnimation();
	//ThrowCrouching
	void HandleCrouchThrowAnimation();
	//ThrowJumping
	void HandleJumpThrowAnimation();

private:
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// Animation variables
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	float
		m_AnimTime,
		m_TotalTimeIdle,
		m_TimeBeforeLongIdle,
		m_FrameSpeed;
	#pragma endregion
};