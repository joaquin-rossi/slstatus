#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>

#include "arg.h"
#include "slstatus.h"
#include "util.h"

typedef struct {
	const char *(*func)();
	const void *arg;
	unsigned int interval; /* interval between updates (in ms) */
	int signo;
} action_t;

#include "config.h"

char *argv0;
char buf[BUFLEN];
char status[STATUSLEN+1];

static Display *dpy;
static volatile sig_atomic_t done;

static void action_run(unsigned ai);
static void handle_action(const int signo);
static void handle_terminate(const int signo);
static struct timespec timespec_from_milli(unsigned int milli);
static void timespec_sub(struct timespec *res, struct timespec *a, struct timespec *b);
static void usage(void);

static struct {
	struct timespec last_time;
	char res[STATUSLEN];
} cache[LEN(actions)] = {0};

int
main(int argc, char **argv)
{
	struct timespec start, end, diff, wait;

	int sflag = 0;
	ARGBEGIN {
		case '1':
			done = 1;
			/* fallthrough */
		case 's':
			sflag = 1;
			break;
		default:
			usage();
	} ARGEND

	if (argc) {
		usage();
	}

	struct sigaction act = {0};
	act.sa_handler = handle_terminate;
	sigaction(SIGINT,  &act, NULL);
	sigaction(SIGTERM, &act, NULL);

	if (!sflag && !(dpy = XOpenDisplay(NULL))) {
		die("XOpenDisplay: Failed to open display");
	}

	for (size_t ai = 0; ai < LEN(actions); ai++) {
		const action_t *a = &actions[ai];

		action_run(ai);
		if (clock_gettime(CLOCK_MONOTONIC, &cache[ai].last_time) < 0)
			die("clock_gettime:");

		if (a->signo != 0) {
			struct sigaction act = {0};
			act.sa_handler = handle_action;
			sigaction(SIGRTMIN + a->signo, &act, NULL);
		}
	}

	do {
		if (clock_gettime(CLOCK_MONOTONIC, &start) < 0)
			die("clock_gettime:");

		size_t pos = 0;
		for (size_t ai = 0, fi = 0; fi < LEN(fmt);) {
			if (pos >= STATUSLEN) {
				pos = STATUSLEN;
				break;
			}

			if (fmt[fi] == '%') {
				if (fi == LEN(actions))
					die("Invalid format string");

				switch (fmt[fi+1]) {
				case '%':
					status[pos++] = '%';
					break;
					
				case '$':
					if (ai >= LEN(actions))
						die("Invalid format string");

					const action_t *a = &actions[ai];
					if (a->interval > 0) {
						struct timespec intspec = timespec_from_milli(a->interval);
						timespec_sub(&diff, &start, &cache[ai].last_time);
						timespec_sub(&diff, &diff, &intspec);

						if (diff.tv_sec >= 0 && diff.tv_nsec >= 0) {
							cache[ai].last_time = start;
							action_run(ai);
						}
					}

					const char *res = cache[ai].res;
					res = res != NULL ? res : unknown_str;

					int len = min(strlen(res), STATUSLEN);
					memcpy(status + pos, res, len);
					pos += len;

					ai++;
					break;

				default:
					die("Invalid format string");
				}

				fi += 2;
			} else {
				status[pos++] = fmt[fi++];
			}
		}

		status[pos] = '\0';

		if (sflag) {
			puts(status);
			fflush(stdout);
			if (ferror(stdout))
				die("puts:");
		} else {
			if (XStoreName(dpy, DefaultRootWindow(dpy), status) < 0)
				die("XStoreName: Allocation failed");
			XFlush(dpy);
		}

		if (!done) {
			if (clock_gettime(CLOCK_MONOTONIC, &end) < 0)
				die("clock_gettime:");

			struct timespec intspec = timespec_from_milli(interval);
			timespec_sub(&diff, &end, &start);
			timespec_sub(&wait, &intspec, &diff);

			if (wait.tv_sec >= 0) {
				if (nanosleep(&wait, NULL) < 0 && errno != EINTR)
					die("nanosleep:");
			}
		}
	} while (!done);

	if (!sflag) {
		XStoreName(dpy, DefaultRootWindow(dpy), NULL);
		if (XCloseDisplay(dpy) < 0)
			die("XCloseDisplay: Failed to close display");
	}

	return 0;
}

void
action_run(unsigned ai)
{
	const action_t *a = &actions[ai];

	const char *r = a->func(a->arg);
	if (r != NULL) strcpy(cache[ai].res, r);
	else strcpy(cache[ai].res, unknown_str);
}

void
handle_action(const int signo)
{
	for (size_t ai = 0; ai < LEN(actions); ai++) {
		const action_t *a = &actions[ai];
		if (SIGRTMIN + a->signo != signo) continue;

		action_run(ai);
		if (clock_gettime(CLOCK_MONOTONIC, &cache[ai].last_time) < 0)
			die("clock_gettime:");

		break;
	}
}

void
handle_terminate(const int signo)
{
	(void) signo;
	done = 1;
}

struct timespec
timespec_from_milli(unsigned int milli)
{
	return (struct timespec){ milli / 1000, (milli % 1000) * 1E6 };
}

void
timespec_sub(struct timespec *res, struct timespec *a, struct timespec *b)
{
	res->tv_sec = a->tv_sec - b->tv_sec - (a->tv_nsec < b->tv_nsec);
	res->tv_nsec = a->tv_nsec - b->tv_nsec + (a->tv_nsec < b->tv_nsec) * 1E9;
}

void
usage(void)
{
	die("usage: %s [-1] [-s]", argv0);
}
