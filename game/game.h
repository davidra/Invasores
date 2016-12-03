#pragma once
/***************************************************************************************************
game.h

by David Ramos
***************************************************************************************************/
#include "game/gameobjectmanager.h"

class IDiceInvaders;

class cGame
{
public:
	static void					initInstance() { DI_assert(!s_instance, "cGame was instantiated already!"); s_instance.reset(new cGame); }
	static cGame&				get() { return *s_instance; }

	void						init(IDiceInvaders* diceInvadersInterface, int width, int height);

	void						update(float elapsed);
	void						draw();

	void						shutDown();

	IDiceInvaders&				getInterface() const { return *m_invadersInterface; }
	cGameObjectManager&			getGameObjectManager() { return m_gameObjectManager; }
	const cGameObjectManager&	getGameObjectManager() const { return m_gameObjectManager; }

	int							getScreenWidth() const { return m_width; }
	int							getScreenHeight() const { return m_height; }

private:
	static std::unique_ptr<cGame> s_instance;

	IDiceInvaders*		m_invadersInterface;
	cGameObjectManager	m_gameObjectManager;

	int					m_width	= 640;
	int					m_height= 480;
};