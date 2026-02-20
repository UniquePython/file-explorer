#include "fs.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdio.h>


static char *make_path(const char *dir, const char *name)
{
    size_t len = strlen(dir) + strlen(name) + 2;
    char *p = malloc(len);
    if (!p) return NULL;
    snprintf(p, len, "%s/%s", dir, name);
    return p;
}


Entry *read_directory(const char *path, size_t *count)
{
    *count = 0;
    DIR *dr = opendir(path);
    if (!dr) return NULL;

    Entry *list = NULL;
    struct dirent *de;

    while ((de = readdir(dr)) != NULL)
    {
        // if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;

        char *full = make_path(path, de->d_name);
        if (!full) continue;

        struct stat st;
        if (stat(full, &st) == -1) {
            free(full);
            continue;
        }

        Entry e = {
            .name = strdup(de->d_name),
            .is_dir = S_ISDIR(st.st_mode)
        };

        Entry *tmp = realloc(list, (*count + 1) * sizeof *list);
        if (!tmp) {
            free(e.name);
            break;
        }

        list = tmp;
        list[(*count)++] = e;
        free(full);
    }

    closedir(dr);
    return list;
}


void free_entries(Entry *entries, size_t count)
{
    for (size_t i = 0; i < count; ++i) free(entries[i].name);
    free(entries);
}