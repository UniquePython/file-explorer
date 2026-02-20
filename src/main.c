#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include "fs.h"
#include "ui.h"


void init_ncurses(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_BLUE, COLOR_GREEN);
    init_pair(3, COLOR_YELLOW, -1);

    refresh();
}


int main(void)
{
    init_ncurses();

    WINDOW *explorer = newwin(LINES - 1, COLS, 0, 0);
    WINDOW *status   = newwin(1, COLS, LINES - 1, 0);

    wbkgd(status, A_REVERSE | COLOR_PAIR(3));

    char *cwd = getcwd(NULL, 0);
    size_t count = 0;
    Entry *entries = cwd ? read_directory(cwd, &count) : NULL;

    if (cwd) mvwprintw(status, 0, 1, "%s", cwd);
    else     mvwprintw(status, 0, 1, "cwd error");

    wrefresh(status);

    if (entries) draw_entries(explorer, entries, count);

    getch();

    free_entries(entries, count);
    free(cwd);
    delwin(explorer);
    delwin(status);
    endwin();
    return 0;
}