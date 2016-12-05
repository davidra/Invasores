#include "pch/pch.h"

#include "game/game.h"
#include "game/playership.h"
#include "game/aliensquadron.h"
#include "game/alien.h"
#include "game/powerup.h"
#include "DiceInvaders.h"

namespace
{
	const cCoord2D s_scorePos = { 440.0f, 0.0f };
	const cCoord2D s_lifeIconsPos = { 300.0f, 0.0f };
	const cCoord2D s_lifeTextPos = s_lifeIconsPos - cCoord2D(56.0f, 0.0f);

	float s_gameOverScreenTime = 4.0f;
	auto s_powerUpRandomDistribution = std::uniform_int_distribution<int>(0, 100);
	int s_powerUpProbabilityPercent = 8;
	float s_rapidFirePowerUpDuration = 5.0f;
	float s_invulnerabilityPowerUpDuration = 5.0f;
}

std::unique_ptr<cGame> cGame::s_instance;

//----------------------------------------------------------------------------
void cGame::init(IDiceInvaders* diceInvadersInterface, int width, int height)
{
	DI_assert(diceInvadersInterface != nullptr, "Fatal error: Null diceInvadersInterface!");
	m_invadersInterface = diceInvadersInterface;

	m_invadersInterface->init(width, height);
	m_width = width;
	m_height = height;

	// create the player's ship
	const tGameObjectId playerShipId = m_gameObjectManager.createGameObject<cPlayerShip>(s_defaultPlayerShipDef, width / 2.0f);
	m_playerShip = static_cast<cPlayerShip*>(m_gameObjectManager.getGameObject(playerShipId));

	// create the alien squadron
	const tGameObjectId alienSquadronId = m_gameObjectManager.createGameObject<cAlienSquadron>(s_defaultAlienSquadron);
	m_alienSquadron = static_cast<cAlienSquadron*>(m_gameObjectManager.getGameObject(alienSquadronId));

	// create the player's life icons
	int iconNum = 0;
	std::generate_n(m_lifeIcons.begin(), PLAYER_LIVES, 
		[this, &iconNum]
		{
			constexpr char LIFE_ICON_SPRITE[] = "data/player.bmp";
			const cCoord2D iconPos = s_lifeIconsPos + cCoord2D(iconNum * static_cast<float>(IDiceInvaders::SPRITE_SIZE), 0.0f);
			const tGameObjectId lifeIconId = m_gameObjectManager.createGameObject<cSprite>(LIFE_ICON_SPRITE, iconPos);

			++iconNum;
			return static_cast<cSprite*>(m_gameObjectManager.getGameObject(lifeIconId));
		});

	m_scoreString.format("SCORE: %08d", 0);
}

//----------------------------------------------------------------------------
void cGame::update(float elapsed)
{
	switch (m_state)
	{
	case cGame::eGameState::START:
	{
		m_alienSquadron->destroyAllAliens();

		m_playerLife = PLAYER_LIVES;
		for (cSprite* const lifeIcon : m_lifeIcons)
		{
			lifeIcon->show();
		}
		m_score = 0;

		m_state = eGameState::PLAY;
	}
	case cGame::eGameState::PLAY:
	{
		m_gameObjectManager.update(elapsed);
	} break;
	case cGame::eGameState::GAMEOVER:
	{
		m_gameOverTimer -= elapsed;

		if (m_gameOverTimer <= 0.0f)
		{
			IDiceInvaders::KeyStatus keyStatus;
			getInterface().getKeyStatus(keyStatus);
			if (keyStatus.fire)
			{
				m_state = eGameState::START;
			}
		}
	}
	break;
	default: break;
	}
	
	updateHUD();
}

//----------------------------------------------------------------------------
void cGame::draw(float elapsed)
{
	m_textPrinter.draw(elapsed);

	m_gameObjectManager.draw(elapsed);
}

//----------------------------------------------------------------------------
void cGame::shutDown()
{
	m_gameObjectManager.destroyAllGameObjects();
	m_invadersInterface->destroy();
}

//----------------------------------------------------------------------------
void cGame::onAlienDestroyed(tGameObjectId alienObjectId)
{
	cAlien* const alien = static_cast<cAlien*>(m_gameObjectManager.getGameObject(alienObjectId));
	m_score += alien->def().scoreReward;

	m_scoreString.format("SCORE: %08d", m_score);

	cFixedString<8> scoreBubbleText; scoreBubbleText.format("%d", alien->def().scoreReward);
	constexpr float SCORE_BUBBLE_DURATION = 1.0f;
	m_textPrinter.AddText(cCoord2D(alien->getPos().x, alien->getPos().y + (IDiceInvaders::SPRITE_SIZE * 0.5f)), scoreBubbleText.c_str(), SCORE_BUBBLE_DURATION);

	// Potentially drop a power up 
	static std::mt19937 mersenne_twister_generator(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	const int randResult = s_powerUpRandomDistribution(mersenne_twister_generator);
	if (randResult < s_powerUpProbabilityPercent)
	{
		constexpr float numTypesOfPowerUp = 3.0f;
		const int powerUpIdx = static_cast<int>((randResult / static_cast<float>(s_powerUpProbabilityPercent)) * numTypesOfPowerUp);
		const cPowerUpDef* powerUp = nullptr;
		switch (powerUpIdx)
		{
		case 0: powerUp = &s_lifeUpPowerUp; break;;
		case 1: powerUp = &s_rapidFirePowerUp; break;
		case 2: powerUp = &s_invulnerabilityPowerUp; break;
		}

		DI_assert(powerUp != nullptr, "Uhm...could not select a proper powerup type for idx %d", powerUpIdx);
		m_gameObjectManager.createGameObject<cPowerUp>(*powerUp, cCoord2D(alien->getPos().x, alien->getPos().y + IDiceInvaders::SPRITE_SIZE));
	}
}

//----------------------------------------------------------------------------
void cGame::onPlayerHit()
{
	m_playerShip->onHit();

	--m_playerLife;
	m_lifeIcons[m_playerLife]->hide();

	if (m_playerLife == 0)
	{
		setGameOver();
	}
}

//----------------------------------------------------------------------------
void cGame::onAlienSquadronLanded()
{
	setGameOver();
}

//----------------------------------------------------------------------------
void cGame::onPowerUp(ePowerUpType type)
{
	switch (type)
	{
	case ePowerUpType::LIFE_UP:
	{
		if (m_playerLife < PLAYER_LIVES)
		{
			m_lifeIcons[m_playerLife]->show();
			++m_playerLife;
		}
	} break;
	case ePowerUpType::RAPID_FIRE:
	{
		m_playerShip->onRapidFirePowerUp(s_rapidFirePowerUpDuration);
	} break;
	case ePowerUpType::INVULNERABILITY:
	{
		m_playerShip->onInvulnerable(s_invulnerabilityPowerUpDuration);
	} break;
	default:
		break;
	}
}

//----------------------------------------------------------------------------
void cGame::updateHUD()
{
	m_textPrinter.AddText(s_scorePos, m_scoreString.c_str());
	m_textPrinter.AddText(s_lifeTextPos, "LIFES:");

	if (m_state == eGameState::GAMEOVER)
	{
		// Make it blink every half second
		const float elapsedFloat = getInterface().getElapsedTime();
		const float decimalPart = elapsedFloat - static_cast<int>(elapsedFloat);
		const cCoord2D gameOverTextPos((getScreenWidth() * 0.5f) - 60.0f, getScreenHeight() * 0.5f);
		if (decimalPart >= 0.5f)
		{
			m_textPrinter.AddText(gameOverTextPos, "GAME OVER!");
		}

		if (m_gameOverTimer <= 0.0f)
		{
			m_textPrinter.AddText(gameOverTextPos + cCoord2D(-32.0f, 16.0f), "PRESS FIRE TO RESTART");
		}
	}
}

//----------------------------------------------------------------------------
void cGame::setGameOver()
{
	m_state = eGameState::GAMEOVER;
	m_gameOverTimer = s_gameOverScreenTime;
}