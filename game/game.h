#pragma once
/***************************************************************************************************
game.h

by David Ramos
***************************************************************************************************/
#include "game/gameobjectmanager.h"
#include "game/textprinter.h"

struct IDiceInvaders;
class cPlayerShip;
class cAlienSquadron;

//----------------------------------------------------------------------------
class cGame
{
public:
	// Singleton interface
	static void					initInstance() { DI_assert(!s_instance, "cGame was instantiated already!"); s_instance.reset(new cGame); }
	static cGame&				get() { return *s_instance; }

	// lifecycle interface
	void						init(IDiceInvaders* diceInvadersInterface, int width, int height);

	void						update(float elapsed);
	void						draw(float elapsed);

	void						shutDown();

	// queries/getters
	IDiceInvaders&				getInterface() const { return *m_invadersInterface; }
	cGameObjectManager&			getGameObjectManager() { return m_gameObjectManager; }
	const cGameObjectManager&	getGameObjectManager() const { return m_gameObjectManager; }

	int							getScreenWidth() const { return m_width; }
	int							getScreenHeight() const { return m_height; }

	cPlayerShip*				getPlayerShip() const { return m_playerShip;  }
	cAlienSquadron*				getAlienSquadron() const { return m_alienSquadron; }

	// Events/messages
	void						onAlienDestroyed(tGameObjectId alienObjectId);
	void						onPlayerHit(unsigned remaining_life);
	void						onAlienSquadronLanded();

private:
	static std::unique_ptr<cGame> s_instance;

	void						updateHUD();

	IDiceInvaders*		m_invadersInterface	= nullptr;
	cGameObjectManager	m_gameObjectManager;
	cTextPrinter		m_textPrinter;

	cPlayerShip*		m_playerShip		= nullptr;
	cAlienSquadron*		m_alienSquadron		= nullptr;

	int					m_width				= 640;
	int					m_height			= 480;

	unsigned			m_score				= 0;
	cFixedString<32>	m_scoreString;
};
