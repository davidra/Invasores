#include "pch/pch.h"

#include "game/gameobjectmanager.h"
#include "game/gameobject.h"

namespace
{
	const size_t INITIAL_OBJECTS_RESERVATION = 200;
}

//----------------------------------------------------------------------------
cGameObjectManager::cGameObjectManager()
{
	// Reserve some generous initial capacity to minimize resizing
	m_gameObjects.reserve(INITIAL_OBJECTS_RESERVATION);
	m_deferredCreationObjects.reserve(20);
}

//----------------------------------------------------------------------------
cGameObjectManager::~cGameObjectManager()
{
	destroyAllGameObjects();
}

//----------------------------------------------------------------------------
void cGameObjectManager::update(float elapsed)
{
	// add those objects which creation was deferred
	for (IGameObject* const gameObject : m_deferredCreationObjects)
	{
		m_gameObjects.push_back(gameObject);
	}
	m_deferredCreationObjects.clear();

	// Update all objects
	m_iteratingObjects = true;
	typedef tGameObjectContainer::iterator tGameObjectIterator;
	tGameObjectIterator itEnd = m_gameObjects.end();
	for (tGameObjectIterator it = m_gameObjects.begin(); it != itEnd; )
	{
		IGameObject* gameObject = *it;

		if (!gameObject->isPendingDestroy())
		{
			gameObject->update(elapsed);
		}

		if (gameObject->isPendingDestroy())
		{
			const bool lastElement = (it + 1) == itEnd;
			if (!lastElement)
			{
				// swap the element with the last one to keep the vector contiguous for convenience
				using std::swap;
				swap(*it, *(itEnd - 1));
			}

			const auto idxOfIt = std::distance(m_gameObjects.begin(), it);

			delete m_gameObjects.back();
			m_gameObjects.pop_back();

			it = m_gameObjects.begin() + idxOfIt;
			itEnd = m_gameObjects.end();
		}
		else
		{
			++it;
		}
	}
	m_iteratingObjects = false;
}

//----------------------------------------------------------------------------
void cGameObjectManager::draw()
{
	m_iteratingObjects = true;
	for (IGameObject* const gameObject : m_gameObjects)
	{
		gameObject->draw();
	}
	m_iteratingObjects = false;
}

//----------------------------------------------------------------------------
void cGameObjectManager::destroyGameObject(tGameObjectId game_object_id)
{
	game_object_id->setPendingDestroy();
}

//----------------------------------------------------------------------------
void cGameObjectManager::destroyAllGameObjects()
{
	while (!m_gameObjects.empty())
	{
		delete m_gameObjects.back();
		m_gameObjects.pop_back();
	}
}

//----------------------------------------------------------------------------
tGameObjectId cGameObjectManager::createGameObjectInternal(IGameObject* newGameObject, const IGameObjectDef& gameObjectDef)
{
	newGameObject->init(gameObjectDef);

	if (!m_iteratingObjects)
	{
		m_gameObjects.push_back(newGameObject);
	}
	else
	{
		m_deferredCreationObjects.push_back(newGameObject);
	}

	return newGameObject;
}
