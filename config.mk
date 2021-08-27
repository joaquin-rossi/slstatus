# paths
PREFIX = /usr/local

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# flags
CPPFLAGS = -I$(X11INC) -D_DEFAULT_SOURCE
CFLAGS   = -pedantic -Wall -Wextra -Os

LDLIBS   = -lX11
# OpenBSD: add -lsndio
# FreeBSD: add -lkvm
LDFLAGS  = -L$(X11LIB) -s $(LDLIBS)

# compiler and linker
CC = gcc
