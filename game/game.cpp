#include "pch/pch.h"

#include "game/game.h"
#include "game/playership.h"
#include "game/aliensquadron.h"
#include "game/alien.h"
#include "DiceInvaders.h"

namespace
{
	const cCoord2D s_scorePos = { 440.0f, 16.0f };
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

	m_scoreString.format("SCORE: %08d", 0);
}

//----------------------------------------------------------------------------
void cGame::update(float elapsed)
{
	m_gameObjectManager.update(elapsed);

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
}

//----------------------------------------------------------------------------
void cGame::onPlayerHit(unsigned remaining_life)
{
}

//----------------------------------------------------------------------------
void cGame::onAlienSquadronLanded()
{
}

//----------------------------------------------------------------------------
void cGame::updateHUD()
{
	m_textPrinter.AddText(s_scorePos, m_scoreString.c_str());
}
