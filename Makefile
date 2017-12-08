CC=gcc
S_SRC= src/main.c src/log.c src/curl.c
S_OBJS=    $(S_SRC:.c=.o)
PKGCONFIG= pkg-config libcurl
OUT=curlutil

CFLAGS+=-Wall -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+=-Wmissing-declarations -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+=-Wsign-compare -Iincludes -g
CFLAGS+=-DPREFIX='"$(PREFIX)"'
CFLAGS+=$(shell $(PKGCONFIG) --cflags)
LDFLAGS=$(shell $(PKGCONFIG) --libs)

all: $(S_OBJS)
	$(CC) $(S_OBJS) $(LDFLAGS) -o $(OUT)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find . -type f -name \*.o -exec rm {} \;
	rm -f $(OUT)
.PHONY: clean
