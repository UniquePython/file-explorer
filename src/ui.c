#include "ui.h"


void draw_entries(WINDOW *win, Entry *e, size_t count)
{
    werase(win);
    box(win, 0, 0);

    int max_row = getmaxy(win) - 1;
    int row = 1;

    for (size_t i = 0; i < count && row < max_row; ++i)
    {
        if (e[i].is_dir) wattron(win, COLOR_PAIR(1) | A_BOLD);

        mvwprintw(win, row++, 1, "%s", e[i].name);

        wattroff(win, COLOR_PAIR(1) | A_BOLD);
    }

    wrefresh(win);
}