#include <ncurses.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


void init_ncurses(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(1, COLOR_BLUE, COLOR_GREEN);  // directories
        init_pair(2, COLOR_WHITE, -1);  // files
        init_pair(3, COLOR_YELLOW, -1); // status
    }

    refresh();
}


char *make_path(const char *dir, const char *name)
{
    size_t len = strlen(dir) + strlen(name) + 2;
    char *full = malloc(len);
    if (!full) return NULL;

    snprintf(full, len, "%s/%s", dir, name);
    return full;
}


void list_files(WINDOW *exp, WINDOW *status, const char *path)
{
    DIR *dr = opendir(path);
    if (!dr) {
        werase(status);
        wbkgd(status, A_REVERSE | COLOR_PAIR(3));
        mvwprintw(status, 0, 1, "Could not open directory: %s", path);
        wrefresh(status);
        return;
    }

    werase(exp);
    box(exp, 0, 0);

    int row = 1;
    int max_row = getmaxy(exp) - 1;

    struct dirent *de;
    while ((de = readdir(dr)) != NULL && row < max_row)
    {
        // if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
        //     continue;

        char *fullpath = make_path(path, de->d_name);
        if (!fullpath) continue;

        struct stat st;
        if (stat(fullpath, &st) == -1) {
            free(fullpath);
            continue;
        }

        if (S_ISDIR(st.st_mode)) wattron(exp, COLOR_PAIR(1) | A_BOLD);
        else                     wattron(exp, COLOR_PAIR(2));

        mvwprintw(exp, row++, 1, "%s", de->d_name);

        wattroff(exp, COLOR_PAIR(1) | A_BOLD);
        wattroff(exp, COLOR_PAIR(2));

        free(fullpath);
    }

    wrefresh(exp);
    closedir(dr);
}


int main(void)
{
    init_ncurses();

    int status_h = 1;
    int explorer_h = LINES - status_h;

    WINDOW *explorer = newwin(explorer_h, COLS, 0, 0);
    WINDOW *status_bar = newwin(status_h, COLS, LINES - 1, 0);

    box(explorer, 0, 0);
    wrefresh(explorer);

    wbkgd(status_bar, A_REVERSE | COLOR_PAIR(3));

    char *cwd = getcwd(NULL, 0);
    if (cwd)
        mvwprintw(status_bar, 0, 1, "%s", cwd);
    else
        mvwprintw(status_bar, 0, 1, "Unable to get current directory");

    wrefresh(status_bar);

    if (cwd)
        list_files(explorer, status_bar, cwd);

    getch();

    free(cwd);
    delwin(explorer);
    delwin(status_bar);
    endwin();
    return 0;
}
