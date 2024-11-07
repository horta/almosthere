CC         ?= gcc
CFLAGS     ?= -std=c11 -Wall -Wextra -O3 -MMD -MP
PREFIX     ?= /usr/local
SRC         = $(filter-out $(wildcard test_*.c),$(wildcard *.c))
OBJ         = $(SRC:.c=.o)
HDR         = $(wildcard athr*.h)
LIB         = libathr.a
TEST_SRC    = $(wildcard test_*.c)
TEST_OBJ    = $(TEST_SRC:.c=.o)
TEST_TARGET = $(basename $(TEST_OBJ))

PKG_CONFIG_FOUND     := $(if $(shell which pkg-config),true,false)
ATHR_TERMINAL_WIN32  := 1
ATHR_TERMINAL_CURSES := 2
ATHR_TERMINAL_IOCTL  := 3
ATHR_TERMINAL        :=
ATHR_OS_WIN32        := 1
ATHR_OS_UNIX         := 2
ATHR_OS              :=

ifndef ($(OS))
  OS=$(shell uname)
endif

ifeq ($(PKG_CONFIG_FOUND),true)
  CURSES_FOUND := $(shell pkg-config --exists ncurses && echo true || echo true)
  CURSES_LIBS  := $(shell pkg-config --libs ncurses)
else
  CURSES_FOUND := false
  CURSES_LIBS  :=
endif

ifeq ($(OS),Windows_NT)
  ATHR_OS       := ATHR_OS_WIN32
  ATHR_TERMINAL := ATHR_TERMINAL_WIN32
else
  ATHR_OS := ATHR_OS_UNIX
  ifeq ($(CURSES_FOUND),true)
    ATHR_TERMINAL := ATHR_TERMINAL_CURSES
  else
    ATHR_TERMINAL := ATHR_TERMINAL_IOCTL
  endif
  SRC := $(filter-out athr_terminal_win32.c,$(SRC))
  HDR := $(filter-out athr_terminal_win32.h,$(SRC))
endif

CFLAGS += -DATHR_TERMINAL_WIN32=$(ATHR_TERMINAL_WIN32)
CFLAGS += -DATHR_TERMINAL_CURSES=$(ATHR_TERMINAL_CURSES)
CFLAGS += -DATHR_TERMINAL_IOCTL=$(ATHR_TERMINAL_IOCTL)
CFLAGS += -DATHR_TERMINAL=$(ATHR_TERMINAL)
CFLAGS += -DATHR_OS_WIN32=$(ATHR_OS_WIN32)
CFLAGS += -DATHR_OS_UNIX=$(ATHR_OS_UNIX)
CFLAGS += -DATHR_OS=$(ATHR_OS)

$(info OS               = $(OS))
$(info PKG_CONFIG_FOUND = $(PKG_CONFIG_FOUND))
$(info CURSES_FOUND     = $(CURSES_FOUND))
$(info ATHR_TERMINAL    = $(ATHR_TERMINAL))
$(info ATHR_OS          = $(ATHR_OS))
$(info CFLAGS           = $(CFLAGS))

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

-include $(SRC:.c=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_TARGET): %: %.o $(LIB)
	$(CC) $(CFLAGS) $< -L. $(CURSES_LIBS) -lathr -lm -o $@

check: $(TEST_TARGET)
	for test in $(TEST_TARGET); do ./$$test || exit 1; done

install: $(LIB) $(HDR)
	@mkdir -p $(PREFIX)/lib $(PREFIX)/include
	install -m 0755 $(LIB) $(PREFIX)/lib/
	install -m 0644 $(HDR) $(PREFIX)/include/

uninstall:
	rm -f $(PREFIX)/lib/$(LIB) $(HDR:%=$(PREFIX)/include/%)

.PHONY: all clean check uninstall
clean:
	rm -f $(OBJ) $(LIB) $(TEST_OBJ) $(TEST_TARGET) *.d
