include config.mk
SRC=$(wildcard src/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

COM =\
	src/components/battery\
	src/components/cpu\
	src/components/datetime\
	src/components/disk\
	src/components/echo\
	src/components/entropy\
	src/components/hostname\
	src/components/ip\
	src/components/kernel_release\
	src/components/keyboard_indicators\
	src/components/keymap\
	src/components/load_avg\
	src/components/netspeeds\
	src/components/num_files\
	src/components/ram\
	src/components/run_command\
	src/components/swap\
	src/components/temperature\
	src/components/uptime\
	src/components/user\
	src/components/volume\
	src/components/wifi

all: slstatus

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

slstatus: $(OBJ) $(COM:=.o)
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f slstatus $(COM:=.o) $(REQ:=.o) $(OBJ)

install: all
	mkdir -p "$(DESTDIR)$(PREFIX)/bin"
	cp -f slstatus "$(DESTDIR)$(PREFIX)/bin"
	chmod 755 "$(DESTDIR)$(PREFIX)/bin/slstatus"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)/bin/slstatus"

.PHONY: all clean install uninstall
