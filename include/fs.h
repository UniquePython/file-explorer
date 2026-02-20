#ifndef FS_H
#define FS_H

#include <stddef.h>

typedef struct {
    char *name;
    int   is_dir;
} Entry;

Entry *read_directory(const char *path, size_t *count);
void   free_entries(Entry *entries, size_t count);

#endif