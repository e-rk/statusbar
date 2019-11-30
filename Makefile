PREFIX = /usr

CFLAGS = -D_POSIX_SOURCE  \
				 -Wall            \
				 -pedantic-errors \
				 -std=c11         \
				 -O2

LDLIBS = -lmpdclient     \
				 -lX11           \
				 -lasound

BIN    = statusbar

OBJ    = statusbar.o     \
				 update_mpd.o    \
				 update_time.o   \
				 update_volume.o \
				 update_battery.o

DEPS   = statusbar.h

$(BIN): $(OBJ)

$(OBJ): $(DEPS)

.PHONY: clean

all: clean $(BIN)

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: install
install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin

