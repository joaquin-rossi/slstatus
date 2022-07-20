#include <stdio.h>

#include "../util.h"

/*
 * - "UNIX is simple and coherent" - Dennis Ritchie, "GNU's Not UNIX" - Richard Stallman
 * - Fun things are fun!
 * - Get in the Robot, Shinji
 * - God's in His Heaven, All's Right With the World
 * - I'd just like to interject for a moment...
 */

const char *
motd(void)
{
    FILE *fp = fopen("/etc/motd", "r");
    if (!fp) return NULL;

    freadln(fp, buf, BUFLEN);
    fclose(fp);

    return buf;
}
