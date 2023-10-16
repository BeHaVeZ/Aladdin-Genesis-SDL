#pragma once
#include "BaseGame.h"
#include <vector>

class Player;
class Level;
class Camera;
class Texture;


class Game : public BaseGame
{
public:
	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update(float elapsedSec) override;
	void Draw() const override;

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	// GameStates
	enum class GameState {
		paused,
		played,
		gameOver,
		finished
	};
	GameState m_CurrentGameState;


	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	Level* m_Level;
	Level* m_BossLevel;
	Camera* m_Camera;
	//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	float		 m_ElapsedFadeTime;
	Rectf		 m_DestRect, m_SrcRect;
	Rectf		 m_NoButton, m_YesButton;
	float		 m_FrameWidth, m_FrameHeight;
	float		 m_AnimTime, m_FrameSpeed;
	float		 m_SoundLength;
	int			 m_NrOfFrames;
	bool		 m_AlreadyPlayed, m_IsGameOverAnimationFinished;
	Point2f	     m_MousePos;
	//------------------------------------------------------------------------------
	void Initialize();
	void Cleanup();
	void ClearBackground() const;
	void ShowControls() const;
	void HandleGameOverSequence(float elapsedSec);
	void HandleFinishedSequence(float elapsedSec);
	void DrawGameOverSequence() const;
	void DrawFinishedSequence() const;

	void ResetLevel();

	//Level is geen base class...
	void InitNextLevel();
};