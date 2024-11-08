ifeq ($(origin CC),default)
  CC := gcc
endif
CFLAGS     ?= -std=c11 -Wall -Wextra -O3 -MMD -MP
PREFIX     ?= /usr/local
SRC         = $(filter-out $(wildcard test_*.c),$(wildcard *.c))
OBJ         = $(SRC:.c=.o)
HDR         = $(wildcard athr*.h)
LIB         = libathr.a
TEST_SRC    = $(wildcard test_*.c)
TEST_OBJ    = $(TEST_SRC:.c=.o)
TEST_TARGET = $(basename $(TEST_OBJ))

PKG_CONFIG_FOUND     := $(if $(shell which pkg-config 2>/dev/null),true,false)

ifeq '$(findstring ;,$(PATH))' ';'
    UNAME := Windows
else
    UNAME := $(shell uname 2>/dev/null || echo Unknown)
    UNAME := $(patsubst CYGWIN%,Cygwin,$(UNAME))
    UNAME := $(patsubst MSYS%,Windows,$(UNAME))
    UNAME := $(patsubst MINGW%,Windows,$(UNAME))
endif

ifeq ($(PKG_CONFIG_FOUND),true)
  CURSES_FOUND := $(shell pkg-config --exists ncurses && echo true || echo false)
  ifeq ($(CURSES_FOUND),true)
    CURSES_LIBS  := $(shell pkg-config --libs ncurses)
  else
    CURSES_LIBS  :=
  endif
else
  CURSES_FOUND := false
  CURSES_LIBS  :=
endif

ifeq ($(UNAME),Windows)
  CFLAGS += -DATHR_OS_WIN32
  CFLAGS += -DATHR_TERMINAL_WIN32
  SRC := $(filter-out athr_terminal_ioctl.c,$(SRC))
  HDR := $(filter-out athr_terminal_ioctl.h,$(SRC))
else
  CFLAGS += -DATHR_OS_UNIX
  ifeq ($(CURSES_FOUND),true)
    CFLAGS += -DATHR_TERMINAL_CURSES
  else
    CFLAGS += -DATHR_TERMINAL_IOCTL
  endif
  SRC := $(filter-out athr_terminal_win32.c,$(SRC))
  HDR := $(filter-out athr_terminal_win32.h,$(SRC))
endif

ifeq ($(CURSES_FOUND),false)
  SRC := $(filter-out athr_terminal_curses.c,$(SRC))
  HDR := $(filter-out athr_terminal_curses.h,$(SRC))
endif

$(info UNAME            = $(UNAME))
$(info CC               = $(CC))
$(info PKG_CONFIG_FOUND = $(PKG_CONFIG_FOUND))
$(info CURSES_FOUND     = $(CURSES_FOUND))
$(info CURSES_LIBS      = $(CURSES_LIBS))
$(info CFLAGS           = $(CFLAGS))

.PHONY: all
all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

-include $(SRC:.c=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_TARGET): %: %.o $(LIB)
	$(CC) $(CFLAGS) $< -L. -lathr $(CURSES_LIBS) -lm -o $@

.PHONY: check
check: $(TEST_TARGET)
	for test in $(TEST_TARGET); do ./$$test || exit 1; done

install: $(LIB) $(HDR)
	@mkdir -p $(PREFIX)/lib $(PREFIX)/include
	install -m 0755 $(LIB) $(PREFIX)/lib/
	install -m 0644 $(HDR) $(PREFIX)/include/

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/lib/$(LIB) $(HDR:%=$(PREFIX)/include/%)

.PHONY: clean
clean:
	rm -f $(OBJ) $(LIB) $(TEST_OBJ) $(TEST_TARGET) *.d
