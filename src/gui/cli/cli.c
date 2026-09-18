#include "cli.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../brick_game/tetris/tetris.h"

enum {
  kFieldHeight = 20,
  kFieldWidth = 10,
  kPreviewSize = 4,
  kFieldTop = 1,
  kFieldLeft = 2,
  kCellWidth = 2,
  kSidebarLeft = 27,
  kMinimumRows = 24,
  kMinimumColumns = 48,
};

static void draw_box(int top, int left, int height, int width) {
  int bottom = top + height + 1;
  int right = left + width + 1;

  mvaddch(top, left, ACS_ULCORNER);
  mvaddch(top, right, ACS_URCORNER);
  mvaddch(bottom, left, ACS_LLCORNER);
  mvaddch(bottom, right, ACS_LRCORNER);

  for (int x = left + 1; x < right; x++) {
    mvaddch(top, x, ACS_HLINE);
    mvaddch(bottom, x, ACS_HLINE);
  }

  for (int y = top + 1; y < bottom; y++) {
    mvaddch(y, left, ACS_VLINE);
    mvaddch(y, right, ACS_VLINE);
  }
}

static void draw_field(const GameInfo_t* info) {
  draw_box(kFieldTop, kFieldLeft, kFieldHeight, kFieldWidth * kCellWidth);

  for (int y = 0; y < kFieldHeight; y++) {
    for (int x = 0; x < kFieldWidth; x++) {
      int screen_y = kFieldTop + 1 + y;
      int screen_x = kFieldLeft + 1 + x * kCellWidth;
      mvaddstr(screen_y, screen_x, info->field[y][x] ? "[]" : "  ");
    }
  }
}

static void draw_next_piece(const GameInfo_t* info) {
  const int next_top = 3;
  const int next_left = kSidebarLeft + 2;

  mvaddstr(next_top - 1, next_left, "NEXT");
  draw_box(next_top, next_left, kPreviewSize, kPreviewSize * kCellWidth);

  for (int y = 0; y < kPreviewSize; y++) {
    for (int x = 0; x < kPreviewSize; x++) {
      int screen_y = next_top + 1 + y;
      int screen_x = next_left + 1 + x * kCellWidth;
      mvaddstr(screen_y, screen_x, info->next[y][x] ? "[]" : "  ");
    }
  }
}

static void draw_sidebar(const GameInfo_t* info) {
  const int sidebar_top = 1;
  const int sidebar_width = 18;

  draw_box(sidebar_top, kSidebarLeft, kFieldHeight, sidebar_width);
  draw_next_piece(info);
  mvprintw(10, kSidebarLeft + 2, "SCORE: %d", info->score);
  mvprintw(12, kSidebarLeft + 2, "HIGH:  %d", info->high_score);
  mvprintw(14, kSidebarLeft + 2, "LEVEL: %d", info->level);
  mvprintw(16, kSidebarLeft + 2, "SPEED: %d", info->speed);

  if (info->pause) {
    mvaddstr(19, kSidebarLeft + 2, "PAUSED");
  }
}

static void draw_game(const GameInfo_t* info) {
  int rows;
  int columns;

  getmaxyx(stdscr, rows, columns);
  erase();

  if (rows < kMinimumRows || columns < kMinimumColumns) {
    mvprintw(0, 0, "Terminal too small: need at least %dx%d", kMinimumColumns,
             kMinimumRows);
  } else {
    mvaddstr(0, kFieldLeft, "TETRIS");
    draw_field(info);
    draw_sidebar(info);
    mvaddstr(23, kFieldLeft, "Enter: start  P: pause  Esc: end  Q: quit");
  }

  refresh();
}

static bool handle_input(void) {
  int key = getch();
  bool running = true;

  switch (key) {
    case KEY_LEFT:
      userInput(Left, false);
      break;
    case KEY_RIGHT:
      userInput(Right, false);
      break;
    case KEY_UP:
      userInput(Up, false);
      break;
    case KEY_DOWN:
      userInput(Down, false);
      break;
    case ' ':
      userInput(Action, false);
      break;
    case '\n':
    case KEY_ENTER:
      userInput(Start, false);
      break;
    case 'p':
    case 'P':
      userInput(Pause, false);
      break;
    case 27:
      userInput(Terminate, false);
      break;
    case 'q':
    case 'Q':
      running = false;
      break;
    default:
      break;
  }

  return running;
}

void cli_run(void) {
  if (initscr() == NULL) {
    fprintf(stderr, "Failed to initialize ncurses\n");
    return;
  }
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  timeout(50);

  while (handle_input()) {
    GameInfo_t info = updateCurrentState();
    draw_game(&info);
  }

  endwin();
}
