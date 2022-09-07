CPPFLAGS = -D_DEFAULT_SOURCE -I/usr/X11R6/include
CFLAGS  = $(CPPFLAGS) -std=c99 -Os -pedantic -Wall -Wextra
LDLIBS  = -lX11
LDFLAGS = -L/usr/X11R6/lib

PREFIX = /usr/local


.PHONY: all clean install uninstall pre-build

all: build

build: pre-build slstatus

clean:
	$(RM) -r build slstatus

install: build
	install -Dm755 slstatus "$(PREFIX)/bin/slstatus"

uninstall:
	$(RM) -r "$(PREFIX)/bin/slstatus"

pre-build:
	@mkdir -p build
	@mkdir -p build/components


build/%.o: src/%.c
	$(CC) $(CFLAGS) $^ -o $@ -c

slstatus: \
		build/slstatus.o \
		build/util.o \
		build/components/battery.o \
		build/components/cpu.o \
		build/components/datetime.o \
		build/components/disk.o \
		build/components/echo.o \
		build/components/entropy.o \
		build/components/hostname.o \
		build/components/ip.o \
		build/components/kernel.o \
		build/components/keyboard.o \
		build/components/load_avg.o \
		build/components/motd.o \
		build/components/netspeeds.o \
		build/components/num_files.o \
		build/components/ram.o \
		build/components/run_command.o \
		build/components/swap.o \
		build/components/temperature.o \
		build/components/uptime.o \
		build/components/user.o \
		build/components/volume.o \
		build/components/wifi.o
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@
