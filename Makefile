MKDIR := mkdir -p
CFLAGS += -Wall -Werror -Wextra -pedantic-errors
CFLAGS += -D_GNU_SOURCE

PROGS := bin/tmpfile\
         bin/syncint\
         bin/largefile\
         bin/ismounted\
         bin/kill_by_pid_file

.PHONY: all clean

all: build

build: $(PROGS)

clean:
	rm -rf $(PROGS)

bin/tmpfile : tmpfile.c rwall.c

bin/syncint: syncint.c
bin/syncint : CFLAGS += -fgnu-tm
bin/syncint : LDFLAGS += -pthread

bin/largefile : tmpfile.c rwall.c rwall.h

bin/ismounted: ismounted.c

bin/kill_by_pid_file: kill_by_pid_file.c

$(PROGS):
	@$(MKDIR) $(dir $@)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

VPATH += src
