#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "options.h"
#include "entry.h"
#include "sort.h"

static const Options *g_opts = NULL;

static time_t get_entry_time(const FileEntry *e, const Options *opts) {
    if (opts->flag_C) return e->st.st_ctime; // Dng flag_C (vit hoa)[cite: 1]
    if (opts->flag_u) return e->st.st_atime; // -u: access time[cite: 2]
    return e->st.st_mtime;
}

static int compare_entries(const void *a, const void *b) {
    const FileEntry *ea = (const FileEntry *)a;
    const FileEntry *eb = (const FileEntry *)b;
    int res = 0;

    if (g_opts->flag_S) {
        if (eb->st.st_size > ea->st.st_size) res = 1;
        else if (eb->st.st_size < ea->st.st_size) res = -1;
        else res = strcmp(ea->name, eb->name);
    } else if (g_opts->flag_t) {
        time_t ta = get_entry_time(ea, g_opts);
        time_t tb = get_entry_time(eb, g_opts);
        if (tb > ta) res = 1;
        else if (tb < ta) res = -1;
        else res = strcmp(ea->name, eb->name);
    } else {
        res = strcmp(ea->name, eb->name);
    }

    if (g_opts->flag_r) {
        res = -res;
    }

    return res;
}

void sort_entry_list(EntryList *list, const Options *opts) {
    if (opts->flag_f) {
        return;
    }
    g_opts = opts;
    qsort(list->entries, list->count, sizeof(FileEntry), compare_entries);
}
