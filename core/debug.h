#pragma once
/***************************************************************************************************
debug.h

Simple set of debug utilities
 
by David Ramos
***************************************************************************************************/
#if defined _DEBUG
#define DI_assert(expr, ...) \
				do																		\
				{																		\
					if (!(expr)) {														\
						debug::WriteLine("Assert in %s(%d)", __FILE__, __LINE__);		\
						debug::WriteLine("\t" #expr " - " __VA_ARGS__);					\
						__debugbreak();													\
					}																	\
				}																		\
				while (false)
#else
#define DI_assert(expr, ...) ((void)(expr))
#endif

//-------------------------------------------------------------------------
namespace debug
{
	void WriteLine(const char* fmt, ...);
}
