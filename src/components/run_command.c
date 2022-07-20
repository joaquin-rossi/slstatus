#include <stdio.h>
#include <string.h>

#include "../util.h"

const char *
run_command(const char *cmd)
{
	FILE *fp = popen(cmd, "r");
	if (!fp) {
		warn("popen '%s':", cmd);
		return NULL;
	}

	char *p = fgets(buf, sizeof(buf) - 1, fp);

	if (pclose(fp) < 0) {
		warn("pclose '%s':", cmd);
		return NULL;
	} else if (!p) {
		return NULL;
	}

	if ((p = strrchr(buf, '\n'))) {
		p[0] = '\0';
	}

	return buf[0] ? buf : NULL;
}
