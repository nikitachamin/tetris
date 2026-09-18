CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Werror -pedantic
CPPFLAGS := -Ibrick_game/tetris -Itests
PREFIX ?= $(HOME)/.local
DESTDIR ?=
BINDIR := $(DESTDIR)$(PREFIX)/bin

NCURSES_CFLAGS := $(shell pkg-config --cflags ncurses 2>/dev/null)
NCURSES_LIBS := $(shell pkg-config --libs ncurses 2>/dev/null || echo -lncurses)
CHECK_LIBS := $(shell pkg-config --libs check 2>/dev/null || echo -lcheck)

LIB_SOURCES := brick_game/tetris/board.c brick_game/tetris/piece.c \
	brick_game/tetris/game.c brick_game/tetris/fsm.c brick_game/tetris/tetris.c
CLI_SOURCES := gui/cli/cli.c gui/cli/main.c
TEST_SOURCES := tests/test_main.c tests/test_board.c tests/test_piece.c \
	tests/test_game.c tests/test_api.c

.PHONY: all install uninstall clean dvi dist test gcov_report format \
	cppcheck valgrind

all: build/tetris

build/tetris: $(LIB_SOURCES) $(CLI_SOURCES)
	@mkdir -p build
	$(CC) $(NCURSES_CFLAGS) $(CPPFLAGS) $(CFLAGS) $(LIB_SOURCES) \
		$(CLI_SOURCES) $(NCURSES_LIBS) -o $@

build/test_runner: $(LIB_SOURCES) $(TEST_SOURCES)
	@mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LIB_SOURCES) $(TEST_SOURCES) \
		$(CHECK_LIBS) -o $@

install: all
	install -d "$(BINDIR)"
	install -m 755 build/tetris "$(BINDIR)/tetris"

uninstall:
	rm -f "$(BINDIR)/tetris"

test: build/test_runner
	BRICKGAME_HIGH_SCORE_FILE="$(abspath build/test_high_score.dat)" \
		./build/test_runner

gcov_report: clean
	@mkdir -p report
	$(CC) $(CPPFLAGS) $(CFLAGS) --coverage $(LIB_SOURCES) $(TEST_SOURCES) \
		$(CHECK_LIBS) -o report/tests
	BRICKGAME_HIGH_SCORE_FILE="$(abspath report/tests.dat)" ./report/tests
	lcov --capture --directory report --output-file report/all.info
	lcov --remove report/all.info --ignore-errors unused '*/tests/*' \
		--output-file report/tetris.info
	genhtml report/tetris.info --output-directory report/html

dvi:
	@mkdir -p build
	@if groff -ms -Tdvi < /dev/null >/dev/null 2>&1; then \
		groff -ms -Tdvi docs/tetris.ms > build/tetris.dvi; \
	else \
		cp docs/tetris.txt build/tetris.txt; \
	fi

dist: all docs/tetris.ms docs/tetris.txt docs/tetris_fsm.dot
	@mkdir -p build/dist
	cp -R Makefile .clang-format brick_game gui tests docs build/dist/
	tar -czf build/brickgame-tetris-1.0.tar.gz -C build/dist .
	@rm -rf build/dist

format:
	clang-format -i $(LIB_SOURCES) $(CLI_SOURCES) $(TEST_SOURCES) \
		brick_game/tetris/*.h gui/cli/*.h tests/*.h

cppcheck:
	cppcheck --enable=all --std=c11 --check-level=exhaustive \
		--disable=information --suppress=missingIncludeSystem \
		--suppress=missingInclude --suppress=checkersReport .

valgrind: build/test_runner
	BRICKGAME_HIGH_SCORE_FILE="$(abspath build/test_high_score.dat)" \
		valgrind --tool=memcheck --leak-check=full --error-exitcode=1 \
		./build/test_runner

clean:
	rm -rf build report