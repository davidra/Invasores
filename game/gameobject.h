#pragma once
/***************************************************************************************************
gameobject.h

Interfaces for base gameobject types
 
by David Ramos
***************************************************************************************************/

#define REGISTER_GAMEOBJECT(go)																							\
public:																													\
	typedef go##Def tGameObjectDef;																						\
	static_assert(std::is_base_of<IGameObjectDef, tGameObjectDef>::value, #go "Def should inherit from IGameObjectDef");\
	const tGameObjectDef& def() const { return static_cast<const tGameObjectDef&>(getDef()); }

//----------------------------------------------------------------------------
struct IGameObjectDef
{
};

//----------------------------------------------------------------------------
struct IGameObject
{
	virtual ~IGameObject() {}

	virtual void init(const IGameObjectDef& gameObjectDef) { m_def = &gameObjectDef; }

	virtual void update(float elapsed) = 0;
	virtual void draw() = 0;

	bool isPendingDestroy() const { return m_isPendingDestroy; }
	void setPendingDestroy() { m_isPendingDestroy = true; }

	const IGameObjectDef& getDef() const { return *m_def; }

private:
	const IGameObjectDef*	m_def				= nullptr;
	bool					m_isPendingDestroy	= false;
};
