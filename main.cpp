#include <ncurses.h>

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    printw("Hello");

    getch();
    endwin();
}
