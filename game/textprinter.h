#pragma once
/***************************************************************************************************
textprinter.h

Class that stores texts requested from gameplay to be drawn, with additional properties like persistence timers, etc.
 
by David Ramos
***************************************************************************************************/
class cTextPrinter
{
public:
	cTextPrinter();

	void draw(float elapsed);
	void AddText(const cCoord2D& pos, const char* str, float persist_timer = 0.0f)
	{
		m_entries[GetCurIdx()].emplace_back(pos, str, persist_timer);
	}

private:
	unsigned	GetCurIdx() const { return m_curIdx; }
	unsigned	GetAltIdx() const { return GetCurIdx() ^ 1; }
	void		ToggleIdx() { m_curIdx ^= 1; }

	struct tTextEntry
	{
		tTextEntry(const cCoord2D& pos, const char* str, float persist_timer = 0.0f)
			: pos(pos)
			, text(str)
			, persist_timer(persist_timer)
		{}

		// Default copy and move constructors are ok

		typedef cFixedString<128> tString;

		cCoord2D	pos;
		tString		text;
		float		persist_timer;
	};

	typedef std::vector<tTextEntry> tEntryContainer;
	tEntryContainer m_entries[2];
	unsigned		m_curIdx = 0;
};