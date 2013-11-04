MKDIR := mkdir -p
CFLAGS := -Wall -Werror -Wextra -pedantic-errors
PROGS := bin/tmpfile
OBJS := $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))

.PHONY: all clean

all: build

build: $(PROGS)

clean:
	rm -rf bin/* obj/*

bin/tmpfile : obj/tmpfile.o obj/rwall.o

$(PROGS):
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

obj/%.o : src/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c -MD -o $@ $<
