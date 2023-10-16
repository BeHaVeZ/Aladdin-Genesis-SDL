#include "pch.h"
#include <string>
#include <iostream>
#include <Texture.h>
#include <utils.h>
#include "Game.h"
#include "Player.h"
#include "Level.h"
#include "Camera.h"
#include "RangedEnemy.h"
#include "SoundManager.h"
#include "TextureManager.h"

Game::Game(const Window& window)
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	m_CurrentGameState = GameState::played;
	m_Level = new Level();
	m_Camera = new Camera(m_Level->GetBoundaries().width, m_Level->GetBoundaries().height);
	m_NrOfFrames = 28;
	m_FrameHeight = 75;
	m_MousePos = Point2f(0, 0);

	m_IsGameOverAnimationFinished = false;
	m_AlreadyPlayed = false;
	m_FrameWidth = m_Level->GetLevelTextureManager()->GetTextureReference(TextureManager::TextureKey::GenieGameOverTxr)->GetWidth() / m_NrOfFrames;
	m_AnimTime = 0;
	m_FrameSpeed = 0.1f;
	m_ElapsedFadeTime = 0;
	m_DestRect = Rectf(GetViewPort().width / 2 - m_FrameWidth / 2, GetViewPort().height / 2 - m_FrameHeight / 2, m_FrameWidth * 2, m_FrameHeight * 2);
	m_SrcRect = Rectf(0, 0, m_FrameWidth, m_FrameHeight);
	m_SoundLength = 5;
	m_NoButton = Rectf(GetViewPort().width * .35f, GetViewPort().height * .3f, 80, 50);
	m_YesButton = Rectf(GetViewPort().width * .55f, GetViewPort().height * .3f, 80, 50);
}

void Game::Cleanup()
{
	delete m_Level;
	delete m_Camera;
}

void Game::Update(float elapsedSec)
{
	SDL_SetWindowTitle(SDL_GL_GetCurrentWindow(), ("1GD23N_Terentyev_Alexander_Aladdin_Genesis"));
	m_AnimTime += elapsedSec;
	if (m_Level->GetPlayer()->GetPlayerHealth() <= 0)
	{
		m_CurrentGameState = GameState::gameOver;
	}
	if (m_Level->IsFinished())
	{
		m_CurrentGameState = GameState::finished;
	}

	switch (m_CurrentGameState)
	{
	case Game::GameState::paused:

		break;
	case Game::GameState::played:
		m_Level->Update(elapsedSec);
		m_Level->HandleCollision();
		m_Camera->Update(*m_Level->GetPlayer()->GetShape());
		break;
	case Game::GameState::gameOver:
		HandleGameOverSequence(elapsedSec);
		break;
	case GameState::finished:
		HandleFinishedSequence(elapsedSec);
	}

}

void Game::Draw() const
{
	switch (m_CurrentGameState)
	{
	case Game::GameState::paused:
		utils::SetColor(Color4f(1, 1, 1, 1));
		ClearBackground();
		glPushMatrix();
		{
			glScalef(2.8f, 2.9f, 0);
			glTranslatef(-m_Camera->GetCameraPos().x, -m_Camera->GetCameraPos().y, 0);
			m_Level->DrawBackGround(m_Camera);
			m_Level->DrawForeGround();
		}
		glPopMatrix();
		m_Level->DrawHud();
		utils::SetColor(Color4f(0.f, 0.f, 0.f, .3f));
		utils::FillRect(0.f, 0.f, GetViewPort().width, GetViewPort().height);
		break;
	case Game::GameState::played:
		ClearBackground();
		glPushMatrix();
		{
			glScalef(2.8f, 2.9f, 0);
			glTranslatef(-m_Camera->GetCameraPos().x, -m_Camera->GetCameraPos().y, 0);
			m_Level->DrawBackGround(m_Camera);
			m_Level->DrawForeGround();
		}
		glPopMatrix();
		m_Level->DrawHud();
		utils::SetColor(Color4f(0.f, 0.f, 0.f, m_ElapsedFadeTime));
		break;
	case Game::GameState::gameOver:
		DrawGameOverSequence();
		break;
	case GameState::finished:
		DrawFinishedSequence();
		break;
	}
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_RETURN:
		if (m_CurrentGameState == GameState::paused)
		{
			m_Level->GetLevelSoundManager()->Resume();
			m_CurrentGameState = GameState::played;
		}
		else
		{
			m_Level->GetLevelSoundManager()->Pause();
			m_CurrentGameState = GameState::paused;
		}
		break;
	case SDLK_s:
		m_Level->GetPlayer()->Throw(m_Level->GetLevelProjectiles());
		break;
	case SDLK_x:
		m_Level->GetPlayer()->Attack();
		break;
	case SDLK_r:
		m_Level->GetPlayer()->ResetPosition();
		break;
	case SDLK_t:
		ResetLevel();
		break;
	case SDLK_b: Log("");
		break;
	case SDLK_o: m_Level->GetPlayer()->Damage();
		break;
	case SDLK_m: m_Level->GetLevelSoundManager()->MuteSound();
		break;
	case SDLK_KP_8:
	case SDLK_PLUS:
	case SDLK_EQUALS:
		m_Level->GetLevelSoundManager()->AddVolume();
		break;
	case SDLK_KP_2:
	case SDLK_MINUS:
		m_Level->GetLevelSoundManager()->SubVolume();
		break;
	case SDLK_i:
		ShowControls();
		break;
	}
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
}

// MOUSE MOTION
void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_MousePos = Point2f((float)e.x, (float)e.y);
}
// MOUSE DOWN
void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	if (m_IsGameOverAnimationFinished == true)
	{
		if (utils::IsPointInRect(m_MousePos, m_NoButton))
		{
			Cleanup();
			SDL_Quit();
			delete this;
			exit(0);
		}
		else if (utils::IsPointInRect(m_MousePos, m_YesButton)) {
			ResetLevel();
		}
	}
}
// MOUSE UP
void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground() const
{
	glClearColor(140 / 255.f, 138 / 255.f, 173 / 255.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::ShowControls() const
{
	Log("--- Controls ---");
	Log("");
	Log("Left / Right arrow keys to move");
	Log("'Z' to jump");
	Log("'X' to swing your sword (melee attack)");
	Log("'S' to throw an apple");
	Log("");
	Log("M to mute the sound");
	Log("2 on numpad or '-' to decrease the sound");
	Log("8 on numpad or '= / +' to increase the sound");
	Log("Enter to pause the game");
}

void Game::HandleGameOverSequence(float elapsedSec)
{
	m_Level->GetLevelSoundManager()->Stop();

	if (m_IsGameOverAnimationFinished == false)
	{
		m_ElapsedFadeTime += elapsedSec;
	}
	else m_ElapsedFadeTime -= elapsedSec;
	if (m_IsGameOverAnimationFinished == false)
	{
		if (m_ElapsedFadeTime > 1)
		{
			if (m_AnimTime >= m_FrameSpeed)
			{
				m_SrcRect.bottom = 0;
				m_SrcRect.left += m_FrameWidth;
				if (m_SrcRect.left >= m_Level->GetLevelTextureManager()->GetTextureReference(TextureManager::TextureKey::GenieGameOverTxr)->GetWidth() - m_FrameWidth * 11 and m_AlreadyPlayed == false)
				{
					m_Level->GetLevelSoundManager()->PlaySoundEffect(SoundManager::SoundEffects::GenieSpawn);
					m_AlreadyPlayed = true;
				}
				if (m_SrcRect.left >= m_Level->GetLevelTextureManager()->GetTextureReference(TextureManager::TextureKey::GenieGameOverTxr)->GetWidth())
				{
					m_SrcRect.left = m_FrameWidth * 24;
				}
				m_AnimTime = 0;
			}
		}
		if (m_ElapsedFadeTime > m_SoundLength)
		{
			m_AlreadyPlayed = false;
			m_IsGameOverAnimationFinished = true;
			m_ElapsedFadeTime = 2;
		}
	}
	else
	{
		if (m_AlreadyPlayed == false)
		{
			m_Level->GetLevelSoundManager()->PlaySoundEffect(SoundManager::SoundEffects::GameOverMusic);
			m_AlreadyPlayed = true;
		}
	}
}
void Game::HandleFinishedSequence(float elapsedSec)
{
	m_Level->GetLevelSoundManager()->Stop();
	if (m_ElapsedFadeTime < 1)
	{
		if (m_AlreadyPlayed == false)
		{
			m_AlreadyPlayed = true;
			m_Level->GetLevelSoundManager()->PlaySoundEffect(SoundManager::SoundEffects::GameOver);
		}
		m_ElapsedFadeTime += elapsedSec;
	}
}
void Game::DrawFinishedSequence() const
{
	utils::SetColor(Color4f(0.f, 0.f, 0.f, m_ElapsedFadeTime));
	utils::FillRect(0.f, 0.f, GetViewPort().width, GetViewPort().height);
	if (m_ElapsedFadeTime >= 1)
	{
		m_Level->GetLevelTextureManager()->GetTextureReference(TextureManager::TextureKey::GameFinished)->Draw(Rectf(0, 0, GetViewPort().width, GetViewPort().height));
	}
}
void Game::DrawGameOverSequence() const
{
	if (m_IsGameOverAnimationFinished == false)
	{
		ClearBackground();
		glPushMatrix();
		{
			glScalef(2.8f, 2.9f, 0);
			glTranslatef(-m_Camera->GetCameraPos().x, -m_Camera->GetCameraPos().y, 0);
			m_Level->DrawBackGround(m_Camera);
			m_Level->DrawForeGround();
		}
		glPopMatrix();
		utils::SetColor(Color4f(0.f, 0.f, 0.f, m_ElapsedFadeTime));
		utils::FillRect(0.f, 0.f, GetViewPort().width, GetViewPort().height);
		if (m_ElapsedFadeTime > 1)
		{
			m_Level->GetLevelTextureManager()->GetTextureReference(TextureManager::TextureKey::GenieGameOverTxr)->Draw(m_DestRect, m_SrcRect);
		}
	}
	else {
		ClearBackground();
		if (utils::IsPointInRect(m_MousePos, m_NoButton))
		{
			m_Level->GetLevelTextureManager()->GetTextureReference(TextureManager::TextureKey::GameOverScreenNo)->Draw(Rectf(0, 0, GetViewPort().width, GetViewPort().height));
		}
		else if (utils::IsPointInRect(m_MousePos, m_YesButton)) {
			m_Level->GetLevelTextureManager()->GetTextureReference(TextureManager::TextureKey::GameOverScreenYes)->Draw(Rectf(0, 0, GetViewPort().width, GetViewPort().height));
		}
		else {
			m_Level->GetLevelTextureManager()->GetTextureReference(TextureManager::TextureKey::GameOverScreen)->Draw(Rectf(0, 0, GetViewPort().width, GetViewPort().height));
		}
		utils::SetColor(Color4f(0.f, 0.f, 0.f, m_ElapsedFadeTime));
		utils::FillRect(0.f, 0.f, GetViewPort().width, GetViewPort().height);
	}
}
void Game::ResetLevel()
{
	Cleanup();
	Initialize();
}
/// <summary>
/// Level is geen base class
/// </summary>
void Game::InitNextLevel()
{
	//m_BossLevel = new Level();
}