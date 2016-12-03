#include "pch/pch.h"

#include "debug.h"

#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <algorithm>

namespace debug {

	//-------------------------------------------------------------------------
	void WriteLine(const char* fmt, ...)
	{
		const int large_enough = 1024;
		char buffer[large_enough] = "";

		va_list args;

		va_start(args, fmt);
		const int vsnprintf_result = vsnprintf_s(buffer, _TRUNCATE, fmt, args);
		const int num_written = vsnprintf_result >= 0 ? vsnprintf_result : large_enough - 1;
		va_end(args);

		const int room_for_new_line_and_terminating_nul = 2;
		const int end_of_str_fix_up_start = (std::min)((std::max)(0, num_written), large_enough - room_for_new_line_and_terminating_nul);
		end_of_str_fix_up_start[buffer] = '\n';
		end_of_str_fix_up_start[buffer + 1] = '\0';

		::OutputDebugStringA(buffer);

		printf("%s", buffer);
	}
}
