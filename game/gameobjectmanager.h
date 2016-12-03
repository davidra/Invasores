#pragma once
/***************************************************************************************************
gameobjectmanager.h

Class manager of game objects
 
by David Ramos
***************************************************************************************************/
class IGameObject;
class IGameObjectDef;

typedef IGameObject* tGameObjectId;
static constexpr tGameObjectId INVALID_GAMEOBJECT_ID = 0;

//----------------------------------------------------------------------------
class cGameObjectManager
{
public:
	cGameObjectManager();
	~cGameObjectManager();

	template <class tGameObject, typename... Args>
	tGameObjectId createGameObject(const typename tGameObject::tGameObjectDef& gameObjectDef, Args&&... constructorArgs);

	void update(float elapsed);
	void draw();

	// A tGameObjectId is actually just the pointer to the game object. In a more fleshed-out system it would be a proper handle
	IGameObject*	getGameObject(tGameObjectId game_object_id) const { return game_object_id; }
	void			destroyGameObject(tGameObjectId game_object_id);
	void			destroyAllGameObjects();

private:
	tGameObjectId createGameObjectInternal(IGameObject* newGameObject, const IGameObjectDef& gameObjectDef);

	typedef std::vector<IGameObject*> tGameObjectContainer;
	tGameObjectContainer m_gameObjects;
	tGameObjectContainer m_deferredCreationObjects;
	
	bool m_iteratingObjects = false;
};

//----------------------------------------------------------------------------
template <class tGameObject, typename... Args>
inline tGameObjectId cGameObjectManager::createGameObject(const typename tGameObject::tGameObjectDef& gameObjectDef, Args&&... constructorArgs)
{
	static_assert(std::is_base_of<IGameObject, tGameObject>::value, "tGameObject should inherit from IGameObject");																					\

	return createGameObjectInternal(new typename tGameObject(std::forward<Args>(constructorArgs)...), gameObjectDef);
}
