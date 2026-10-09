#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "entry.h"
#include "sort.h"
#include "display.h"

void init_entry_list(EntryList *list) {
    list->count = 0;
    list->capacity = 16;
    list->total_blocks = 0;
    list->entries = malloc(list->capacity * sizeof(FileEntry));
}

void add_entry(EntryList *list, FileEntry entry) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->entries = realloc(list->entries, list->capacity * sizeof(FileEntry));
    }
    list->entries[list->count++] = entry;
    list->total_blocks += entry.st.st_blocks;
}

void free_entry_list(EntryList *list) {
    for (int i = 0; i < list->count; i++) {
        free(list->entries[i].name);
        free(list->entries[i].path);
    }
    free(list->entries);
    list->entries = NULL;
    list->count = 0;
}

void process_path(const char *path, const Options *opts, int print_hdr) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        perror(path);
        return;
    }

    if (opts->flag_d || !S_ISDIR(st.st_mode)) {
        EntryList list;
        init_entry_list(&list);

        FileEntry fe = {0};
        fe.name = strdup(path);
        fe.path = strdup(path);
        fe.st = st;
        if (S_ISLNK(st.st_mode)) {
            fe.is_symlink = 1;
            ssize_t len = readlink(path, fe.link_target, sizeof(fe.link_target) - 1);
            if (len != -1) fe.link_target[len] = '\0';
        }
        add_entry(&list, fe);
        display_entries(&list, opts, 0);
        free_entry_list(&list);
        return;
    }

    DIR *dir = opendir(path);
    if (!dir) {
        perror(path);
        return;
    }

    if (print_hdr) {
        printf("%s:\n", path);
    }

    EntryList list;
    init_entry_list(&list);

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            if (!opts->flag_a && !opts->flag_A) continue;
            if (opts->flag_A && !opts->flag_a) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            }
        }

        char fullpath[1024];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", strcmp(path, "/") == 0 ? "" : path, entry->d_name);

        FileEntry fe = {0};
        fe.name = strdup(entry->d_name);
        fe.path = strdup(fullpath);

        if (lstat(fullpath, &fe.st) == -1) {
            free(fe.name);
            free(fe.path);
            continue;
        }

        if (S_ISLNK(fe.st.st_mode)) {
            fe.is_symlink = 1;
            ssize_t len = readlink(fullpath, fe.link_target, sizeof(fe.link_target) - 1);
            if (len != -1) fe.link_target[len] = '\0';
        }

        add_entry(&list, fe);
    }
    closedir(dir);

    sort_entry_list(&list, opts);
    display_entries(&list, opts, 1);

    if (opts->flag_R) {
        for (int i = 0; i < list.count; i++) {
            FileEntry *fe = &list.entries[i];
            if (S_ISDIR(fe->st.st_mode)) {
                if (strcmp(fe->name, ".") != 0 && strcmp(fe->name, "..") != 0) {
                    printf("\n");
                    process_path(fe->path, opts, 1);
                }
            }
        }
    }

    free_entry_list(&list);
}
