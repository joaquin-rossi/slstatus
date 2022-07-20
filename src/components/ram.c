#include <stdio.h>

#include "../util.h"

#if defined(__linux__)
	#include <stdint.h>

	const char *
	ram_free(void)
	{
		uintmax_t free;
		if (pscanf("/proc/meminfo",
				   "MemTotal: %ju kB\n"
				   "MemFree: %ju kB\n"
				   "MemAvailable: %ju kB\n",
				   &free, &free, &free) != 3) {
			return NULL;
		}

		return fmt_human(free * 1024, 1024);
	}

	const char *
	ram_perc(void)
	{
		uintmax_t total, free, buffers, cached;
		if (pscanf("/proc/meminfo",
				   "MemTotal: %ju kB\n"
				   "MemFree: %ju kB\n"
				   "MemAvailable: %ju kB\n"
				   "Buffers: %ju kB\n"
				   "Cached: %ju kB\n",
				   &total, &free, &buffers, &buffers, &cached) != 5) {
			return NULL;
		}

		if (total == 0) {
			return NULL;
		}

		return bprintf("%d", 100 * ((total - free) - (buffers + cached)) / total);
	}

	const char *
	ram_total(void)
	{
		uintmax_t total;
		return pscanf("/proc/meminfo", "MemTotal: %ju kB\n", &total) != 1
			? NULL
			: fmt_human(total * 1024, 1024)
			;
	}

	const char *
	ram_used(void)
	{
		uintmax_t total, free, buffers, cached;

		if (pscanf("/proc/meminfo",
				   "MemTotal: %ju kB\n"
				   "MemFree: %ju kB\n"
				   "MemAvailable: %ju kB\n"
				   "Buffers: %ju kB\n"
				   "Cached: %ju kB\n",
				   &total, &free, &buffers, &buffers, &cached) != 5) {
			return NULL;
		}

		return fmt_human((total - free - buffers - cached) * 1024, 1024);
	}
#elif defined(__OpenBSD__)
	#include <stdlib.h>
	#include <sys/sysctl.h>
	#include <sys/types.h>
	#include <unistd.h>

	#define LOG1024 10
	#define pagetok(size, pageshift) (size_t)(size << (pageshift - LOG1024))

	inline int
	load_uvmexp(struct uvmexp *uvmexp)
	{
		int uvmexp_mib[] = {CTL_VM, VM_UVMEXP};
		size_t size = sizeof(*uvmexp);

		return sysctl(uvmexp_mib, 2, uvmexp, &size, NULL, 0) >= 0
			? 1
			: 0
			;
	}

	const char *
	ram_free(void)
	{
		struct uvmexp uvmexp;
		return load_uvmexp(&uvmexp)
			? fmt_human(pagetok(uvmexp.npages - uvmexp.active, uvmexp.pageshift) * 1024, 1024)
			: NULL;
	}

	const char *
	ram_perc(void)
	{
		struct uvmexp uvmexp;
		return load_uvmexp(&uvmexp)
			? bprintf("%d", uvmexp.active * 100 / uvmexp.npages)
			: NULL
			;
	}

	const char *
	ram_total(void)
	{
		struct uvmexp uvmexp;
		return load_uvmexp(&uvmexp)
			? fmt_human(pagetok(uvmexp.npages, uvmexp.pageshift) * 1024, 1024)
			: NULL
			;
	}

	const char *
	ram_used(void)
	{
		struct uvmexp uvmexp;
		return load_uvmexp(&uvmexp)
			? fmt_human(pagetok(uvmexp.active, uvmexp.pageshift) * 1024, 1024)
			: NULL
			;
	}
#elif defined(__FreeBSD__)
	#include <sys/sysctl.h>
	#include <sys/vmmeter.h>
	#include <unistd.h>
	#include <vm/vm_param.h>

	const char *
	ram_free(void)
	{
		struct vmtotal vm_stats;
		int mib[] = {CTL_VM, VM_TOTAL};

		size_t len = sizeof(struct vmtotal);
		if (sysctl(mib, 2, &vm_stats, &len, NULL, 0) == -1 || !len)
			return NULL;

		return fmt_human(vm_stats.t_free * getpagesize(), 1024);
	}

	const char *
	ram_total(void)
	{
		size_t len = sizeof(npages);

		long npages;
		if (sysctlbyname("vm.stats.vm.v_page_count", &npages, &len, NULL, 0) == -1 || !len)
			return NULL;

		return fmt_human(npages * getpagesize(), 1024);
	}

	const char *
	ram_perc(void)
	{
		size_t len = sizeof(npages);

		long active;
		if (sysctlbyname("vm.stats.vm.v_active_count", &active, &len, NULL, 0) == -1 || !len)
			return NULL;

		long npages;
		if (sysctlbyname("vm.stats.vm.v_page_count", &npages, &len, NULL, 0) == -1 || !len)
			return NULL;

		return bprintf("%d", active * 100 / npages);
	}

	const char *
	ram_used(void) {
		size_t len = sizeof(active);

		long active;
		if (sysctlbyname("vm.stats.vm.v_active_count", &active, &len, NULL, 0) == -1 || !len)
			return NULL;

		return fmt_human(active * getpagesize(), 1024);
	}
#endif
