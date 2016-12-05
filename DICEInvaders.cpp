#include "pch/pch.h"

#include "DiceInvaders.h"
#include "game/game.h"

namespace
{
	const int DEFAULT_WIDTH = 640;
	const int DEFAULT_HEIGHT = 480;

	float debug_time_scale = 1.0f;
}

class DiceInvadersLib
{
public:
	explicit DiceInvadersLib(const char* libraryPath)
	{
		m_lib = LoadLibrary(libraryPath);
		DI_assert(m_lib, "Error loading library %s", libraryPath);

		DiceInvadersFactoryType* factory = (DiceInvadersFactoryType*)GetProcAddress(
				m_lib, "DiceInvadersFactory");
		m_interface = factory();
		DI_assert(m_interface, "The factory did not return a valid interface pointer");
	}

	~DiceInvadersLib()
	{
		FreeLibrary(m_lib);
	}

	IDiceInvaders* get() const 
	{
		return m_interface;
	}

	DiceInvadersLib(const DiceInvadersLib&) = delete;
	DiceInvadersLib& operator=(const DiceInvadersLib&) = delete;

private: 
	IDiceInvaders* m_interface;
	HMODULE m_lib;
};

//----------------------------------------------------------------------------
int APIENTRY WinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	LPSTR commandLine,
	int commandShow)
{
	cGame::initInstance();
	cGame& invadersGame = cGame::get();

	DiceInvadersLib lib("DiceInvaders.dll");

	invadersGame.init(lib.get(), DEFAULT_WIDTH, DEFAULT_HEIGHT);
	IDiceInvaders& system = invadersGame.getInterface();

	float lastTime = system.getElapsedTime();
	while (system.update())
	{
		const float newTime = system.getElapsedTime();
		const float elapsed = (newTime - lastTime) * debug_time_scale;
		lastTime = newTime;

		invadersGame.update(elapsed);
		invadersGame.draw(elapsed);
	}

	invadersGame.shutDown();

	return 0;
}

/*
int APIENTRY WinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	LPSTR commandLine,
	int commandShow)
{
	DiceInvadersLib lib("DiceInvaders.dll");
	IDiceInvaders* system = lib.get();

	system.init(640, 480);

	cGame invadersGame;

	invadersGame.Init();

	ISprite* sprite = system.createSprite("data/player.bmp");

	float horizontalPosition = 320;
	float lastTime = system.getElapsedTime();
	while (system.update())
	{
		sprite->draw(int(horizontalPosition), 480-32);

		float newTime = system.getElapsedTime();
		float move = (newTime - lastTime) * 160.0f;
		lastTime = newTime;
		
		IDiceInvaders::KeyStatus keys;
		system.getKeyStatus(keys);
		if (keys.right)
			horizontalPosition += move;
		else if (keys.left)
			horizontalPosition -= move;
	}

	sprite->destroy();
	system.destroy();

	return 0;
}
*/



