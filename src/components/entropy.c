#if defined(__linux__)
	#include <stdint.h>
	#include <stdio.h>

	#include "../util.h"

	const char *
	entropy(void)
	{
		uintmax_t num;
		return pscanf("/proc/sys/kernel/random/entropy_avail", "%ju", &num) != 1
			? NULL
			: bprintf("%ju", num)
			;
	}
#elif defined(__OpenBSD__) | defined(__FreeBSD__)
	const char *
	entropy(void)
	{
		/* Unicode Character 'INFINITY' (U+221E) */
		return "\xe2\x88\x9e";
	}
#endif
