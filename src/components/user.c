#include <stdio.h>

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include "../util.h"

const char *
gid(void)
{
	return bprintf("%d", getgid());
}

const char *
uid(void)
{
	return bprintf("%d", geteuid());
}

const char *
username(void)
{
	struct passwd *pw = getpwuid(geteuid());
	if (!pw) {
		warn("getpwuid '%d':", geteuid());
		return NULL;
	}

	return bprintf("%s", pw->pw_name);
}
