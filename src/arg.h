#ifndef ARG_H
#define ARG_H

extern char *argv0;

/* int main(int argc, char **argv) */
#define ARGBEGIN \
	argv0 = *argv;									\
	for (*argv ? (argc--, argv++) : ((void *)0);	\
		 *argv && (*argv)[0] == '-' && (*argv)[1];	\
		 argc--, argv++) {							\
	int i_, argused_;								\
	if ((*argv)[1] == '-' && !(*argv)[2]) {			\
		argc--, argv++;								\
		break;										\
	}												\
	for (i_ = 1, argused_ = 0; (*argv)[i_]; i_++) {	\
	switch((*argv)[i_])

#define ARGEND \
	if (argused_) {				\
		if ((*argv)[i_ + 1]) {	\
			break;				\
		} else {				\
			argc--, argv++;		\
			break;				\
		}						\
	}							\
	}}

#endif /* ARG_H */
