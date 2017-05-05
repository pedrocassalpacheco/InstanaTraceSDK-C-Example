program_NAME := bin/httpserver.out

SRCS = src/http.c src/server.c src/instana.c src/util.c
OBJS := ${SRCS:.c=.o}

CFLAGS += -m64 -std=gnu99 -g

program_INCLUDE_DIRS := include
program_LIBRARY_DIRS :=
program_LIBRARIES := curl

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

CC=gcc

LINK.c := $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

.PHONY: all

all: $(program_NAME)

$(program_NAME): $(OBJS)
	$(LINK.c) $(OBJS) -o $(program_NAME)

clean :
	rm -f src/*.o
