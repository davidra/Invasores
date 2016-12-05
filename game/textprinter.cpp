#include "pch/pch.h"

#include "game/game.h"
#include "game/textprinter.h"
#include "DiceInvaders.h"

namespace
{
	constexpr size_t INITIAL_ENTRIES_CAPACITY = 100;
}

//----------------------------------------------------------------------------
cTextPrinter::cTextPrinter()
{
	m_entries[0].reserve(INITIAL_ENTRIES_CAPACITY);
	m_entries[1].reserve(INITIAL_ENTRIES_CAPACITY);
}

//----------------------------------------------------------------------------
void cTextPrinter::draw(float elapsed)
{
	IDiceInvaders& invadersInterface = cGame::get().getInterface();
	for (tTextEntry& entry : m_entries[GetCurIdx()])
	{
		invadersInterface.drawText(static_cast<int>(entry.pos.x), static_cast<int>(entry.pos.y), entry.text.c_str());
		entry.persist_timer -= elapsed;
		if (entry.persist_timer > 0.0f)
		{
			// Make sure it stays alive for next frame
			// Note: if this happens very often, tTextEntry::tString should be changed to be a std::string, 
			// so moving the entries to the next frame's container is cheaper
			m_entries[GetAltIdx()].push_back(std::move(entry));
		}
	}
	m_entries[GetCurIdx()].clear();

	ToggleIdx();
}
