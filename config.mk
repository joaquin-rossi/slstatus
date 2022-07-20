# paths
PREFIX = /usr/local

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# flags
CPPFLAGS = -I$(X11INC) -D_DEFAULT_SOURCE
CCFLAGS  = -pedantic -Wall -Wextra -O2

# OpenBSD: add -lsndio
# FreeBSD: add -lkvm
LDLIBS  = -lX11
LDFLAGS = -L$(X11LIB) -s $(LDLIBS)
