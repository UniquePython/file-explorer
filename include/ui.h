#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include "fs.h"

void draw_entries(WINDOW *win, Entry *entries, size_t count);

#endif