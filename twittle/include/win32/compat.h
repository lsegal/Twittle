#pragma once

#ifdef _MSC_VER // VS.NET does not have strtoull
#	define strtoull(v, UNUSED, base) _atoi64(v)
#endif
