#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "../util.h"

const char *
num_files(const char *path)
{
	struct dirent *dp;
	DIR *fd;

	if (!(fd = opendir(path))) {
		warn("opendir '%s':", path);
		return NULL;
	}

	int num = 0;
	while ((dp = readdir(fd))) {
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) {
			continue; /* skip self and parent */
		}
		num++;
	}

	closedir(fd);

	return bprintf("%d", num);
}
