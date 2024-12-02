#include <chrono>
#include <clocale>
#include <cstdlib>
#include <ncurses.h>
#include <utility>
#include <vector>

#define Framerate 40

int main(void) {

  MEVENT mouseEvent;
  int c;
  std::vector<std::pair<int, int>> pixels;

  std::chrono::milliseconds loopTime{1000 / Framerate};
  std::chrono::time_point<std::chrono::system_clock> lastTime =
      std::chrono::system_clock::now();

  setlocale(LC_ALL, "");
  initscr();
  clear();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  halfdelay(1);

  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

  while (1) {

    if (std::chrono::system_clock::now().time_since_epoch() -
            lastTime.time_since_epoch() >
        loopTime) {
      c = getch();
      clear();
      box(stdscr, 0, 0);

      if (c == KEY_MOUSE) {

        if (getmouse(&mouseEvent) == OK) {
          if (mouseEvent.bstate & BUTTON1_PRESSED) {
            pixels.push_back(std::make_pair(mouseEvent.y, mouseEvent.x));
          }
        }
      }

      for (auto i = pixels.begin(); i != pixels.end(); i++) {
        mvaddwstr(i->first, i->second, L"\u2588");
        if (i->first < LINES - 2) {
          bool toInc = true;
          for (auto j = pixels.begin(); j != pixels.end(); j++) {
            if (i == j)
              continue;
            if ((j->second == i->second) && (i->first + 1 == j->first)) {
              toInc = false;
            }
          }
          if (toInc)
            i->first++;
        }
      }
      move(LINES, COLS);
      refresh();
      lastTime = std::chrono::system_clock::now();
    }
  }

  endwin();
  return 0;
}
