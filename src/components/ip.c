#include <ifaddrs.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#if defined(__OpenBSD__)
	#include <sys/socket.h>
	#include <sys/types.h>
#elif defined(__FreeBSD__)
	#include <netinet/in.h>
	#include <sys/socket.h>
#endif

#include "../util.h"

static const char *ip(const char *interface, unsigned short sa_family);

const char *
ipv4(const char *interface)
{
	return ip(interface, AF_INET);
}

const char *
ipv6(const char *interface)
{
	return ip(interface, AF_INET6);
}

static const char *
ip(const char *interface, unsigned short sa_family)
{
	char host[NI_MAXHOST];
	struct ifaddrs *ifaddr;

	if (getifaddrs(&ifaddr) < 0) {
		warn("getifaddrs:");
		return NULL;
	}

	for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr) {
			continue;
		}

		int s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in6), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
		if (!strcmp(ifa->ifa_name, interface) && (ifa->ifa_addr->sa_family == sa_family)) {
			freeifaddrs(ifaddr);
			if (s != 0) {
				warn("getnameinfo: %s", gai_strerror(s));
				return NULL;
			}
			return bprintf("%s", host);
		}
	}

	freeifaddrs(ifaddr);
	return NULL;
}
