#ifndef SORT_H
#define SORT_H

#include "options.h"

struct EntryList;
typedef struct EntryList EntryList;

void sort_entry_list(EntryList *list, const Options *opts);

#endif
