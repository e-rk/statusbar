PREFIX = /usr
CC = gcc

CFLAGS = -D_POSIX_SOURCE  \
				 -Wall            \
				 -pedantic-errors \
				 -std=c11

LIBS   = -lmpdclient     \
				 -lX11           \
				 -lasound


BIN    = statusbar

OBJ    = statusbar.o     \
				 update_mpd.o    \
				 update_time.o   \
				 update_volume.o

DEPS  =  statusbar.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(FLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

all: clean $(BIN)

clean:
	rm -f *.o $(BIN)

.PHONY: install
install: $(BIN)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin

