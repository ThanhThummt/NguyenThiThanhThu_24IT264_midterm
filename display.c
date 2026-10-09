#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "display.h"

static void format_mode(mode_t mode, char *str) {
    if (S_ISDIR(mode))  str[0] = 'd';
    else if (S_ISLNK(mode)) str[0] = 'l';
    else if (S_ISCHR(mode)) str[0] = 'c';
    else if (S_ISBLK(mode)) str[0] = 'b';
    else if (S_ISFIFO(mode)) str[0] = 'p';
    else if (S_ISSOCK(mode)) str[0] = 's';
    else str[0] = '-';

    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    if (mode & S_ISUID) str[3] = (mode & S_IXUSR) ? 's' : 'S';
    else str[3] = (mode & S_IXUSR) ? 'x' : '-';

    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    if (mode & S_ISGID) str[6] = (mode & S_IXGRP) ? 's' : 'S';
    else str[6] = (mode & S_IXGRP) ? 'x' : '-';

    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    if (mode & S_ISVTX) str[9] = (mode & S_IXOTH) ? 't' : 'T';
    else str[9] = (mode & S_IXOTH) ? 'x' : '-';

    str[10] = '\0';
}

static void print_human_size(off_t bytes) {
    const char *units[] = {"B", "K", "M", "G", "T"};
    double size = (double)bytes;
    int idx = 0;
    while (size >= 1024.0 && idx < 4) {
        size /= 1024.0;
        idx++;
    }
    if (idx == 0) printf("%5lldB ", (long long)bytes);
    else printf("%5.1f%s ", size, units[idx]);
}

static void print_name(const char *name, const Options *opts) {
    for (size_t i = 0; i < strlen(name); i++) {
        unsigned char c = name[i];
        if (!isprint(c) && opts->flag_q) {
            putchar('?');
        } else {
            putchar(c);
        }
    }
}

static char get_type_indicator(mode_t mode) {
    if (S_ISDIR(mode)) return '/';
    if (S_ISLNK(mode)) return '@';
    if (S_ISSOCK(mode)) return '=';
    if (S_ISFIFO(mode)) return '|';
    if (mode & (S_IXUSR | S_IXGRP | S_IXOTH)) return '*';
    return '\0';
}

void display_entries(const EntryList *list, const Options *opts, int show_total) {
    if (show_total && (opts->flag_l || opts->flag_n || opts->flag_s)) {
        long total = list->total_blocks;
        if (opts->flag_k) total = (total + 1) / 2;
        printf("total %ld\n", total);
    }

    for (int i = 0; i < list->count; i++) {
        FileEntry *e = &list->entries[i];

        if (opts->flag_i) {
            printf("%8llu ", (unsigned long long)e->st.st_ino);
        }

        if (opts->flag_s) {
            long blocks = e->st.st_blocks;
            if (opts->flag_k) blocks = (blocks + 1) / 2;
            printf("%4ld ", blocks);
        }

        if (opts->flag_l || opts->flag_n) {
            char mode_str[11];
            format_mode(e->st.st_mode, mode_str);
            printf("%s %2u ", mode_str, (unsigned int)e->st.st_nlink);

            struct passwd *pw = getpwuid(e->st.st_uid);
            struct group *gr = getgrgid(e->st.st_gid);

            if (!opts->flag_n && pw) printf("%-8s ", pw->pw_name);
            else printf("%-8u ", (unsigned int)e->st.st_uid);

            if (!opts->flag_n && gr) printf("%-8s ", gr->gr_name);
            else printf("%-8u ", (unsigned int)e->st.st_gid);

            if (S_ISCHR(e->st.st_mode) || S_ISBLK(e->st.st_mode)) {
                printf("%3d, %3d ", major(e->st.st_rdev), minor(e->st.st_rdev));
            } else if (opts->flag_h) {
                print_human_size(e->st.st_size);
            } else {
                printf("%8lld ", (long long)e->st.st_size);
            }

            time_t t = e->st.st_mtime;
            if (opts->flag_C) t = e->st.st_ctime; // Sa flag_C (ch C hoa)
            if (opts->flag_u) t = e->st.st_atime;

            char time_buf[64];
            struct tm *tm_info = localtime(&t);
            strftime(time_buf, sizeof(time_buf), "%b %e %H:%M", tm_info);
            printf("%s ", time_buf);
        }

        print_name(e->name, opts);

        if (opts->flag_F) {
            char ind = get_type_indicator(e->st.st_mode);
            if (ind) putchar(ind);
        }

        if ((opts->flag_l || opts->flag_n) && e->is_symlink) {
            printf(" -> %s", e->link_target);
        }

        putchar('\n');
    }
}
