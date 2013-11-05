MKDIR := mkdir -p
CFLAGS += -Wall -Werror -Wextra -pedantic-errors
PROGS := bin/tmpfile\
         bin/syncint

.PHONY: all clean

all: build

build: $(PROGS)

clean:
	rm -rf bin/* obj/*

bin/tmpfile : obj/tmpfile.o obj/rwall.o

bin/syncint : obj/syncint.o
bin/syncint : CFLAGS += -D_GNU_SOURCE -fgnu-tm
bin/syncint : LDFLAGS += -pthread

$(PROGS):
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

obj/%.o : src/%.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c -MD -o $@ $<
