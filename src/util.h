#include <stdint.h>
#include <stdio.h>

#define BUFLEN 1024
extern char buf[BUFLEN];

#define LEN(X) (sizeof X / sizeof X[0])

extern char *argv0;

int min(int x, int y);
int max(int x, int y);

void warn(const char *, ...);
void die(const char *, ...);

const char *bprintf(const char *fmt, ...);
const char *fmt_human(uintmax_t num, int base);
int esnprintf(char *str, size_t size, const char *fmt, ...);
int freadln(FILE *fp, char *buf, int max);
int pscanf(const char *path, const char *fmt, ...);
