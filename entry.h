
#ifndef ENTRY_H
#define ENTRY_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "options.h"

typedef struct {
    char *name;
    char *path;
    struct stat st;
    char link_target[1024];
    int is_symlink;
} FileEntry;

typedef struct EntryList {
    FileEntry *entries;
    int count;
    int capacity;
    long total_blocks;
} EntryList;

void init_entry_list(EntryList *list);
void add_entry(EntryList *list, FileEntry entry);
void free_entry_list(EntryList *list);

void process_path(const char *path, const Options *opts, int print_hdr);

#endif
