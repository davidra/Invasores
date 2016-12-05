#pragma once
/***************************************************************************************************
fixedstring.h

Fixed-size string class that stores the string as an array internally and does not allocate dynamically
only supports single-byte for simplicity. Implements a sub-set of the STL's string class and adds some 
new (like format)
 
by David Ramos
***************************************************************************************************/
template <size_t kSize>
class cFixedString
{
	static_assert(kSize > 0, "Can't have a fixed string with 0 size. We need at least one character for the end of string character");

public:
	void resize(size_t size)
	{
		DI_assert(size < kSize, "Can't set a size bigger than kSize - 1. Fixed strings can't grow");

		m_size = size;
		m_storage[size] = '\0';
	}

	void clear()
	{
		resize(0);
	}

	size_t size() const { return m_size; }
	size_t length() const { return size(); }

	cFixedString& assign(const char* str)
	{
		const size_t newStrLen = (std::min)(strlen(str), kSize - 1);
		strncpy_s(m_storage, str, newStrLen);
		resize(newStrLen);

		return *this;
	}

	template<size_t kSrcSize>
	cFixedString& assign(const cFixedString<kSrcSize>& str)
	{
		assign(str.c_str());
	}

	cFixedString& operator= (const char* str) { return assign(str); }
	template <size_t kSrcSize>
	cFixedString& operator= (const cFixedString<kSrcSize>& str) { return assign(str); }

	cFixedString() : m_storage{""} {}

	cFixedString(const char* str)
	{
		*this = str;
	}

	template<size_t kSrcSize>
	cFixedString(const cFixedString<kSrcSize>& str)
	{
		*this = str;
	}

	cFixedString& formatv(const char* format, va_list args)
	{
		if (!format || (format[0] == '\0'))
		{
			clear();
			return *this;
		}

		const size_t strLen = vsnprintf_s(m_storage, _TRUNCATE, format, args);
		resize((strLen == -1) ? (kSize - 1) : strLen);

		return *this;
	}

	cFixedString& format(const char* format, ...)
	{
		va_list args;

		va_start(args, format);
		formatv(format, args);
		va_end(args);

		return *this;
	}

	char* data() { return m_storage; }
	const char* data() const { return m_storage; }
	const char* c_str() const { return data(); }

private:
	char	m_storage[kSize] = "";
	size_t	m_size = 0;
};