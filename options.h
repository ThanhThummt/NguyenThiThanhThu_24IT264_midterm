#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct {
    int flag_A; // -A: hin file n tr . v ..
    int flag_a; // -a: hin tt c file n
    int flag_C; // -C: dng status change time (ctime)[cite: 1]
    int flag_d; // -d: xem th mc nh file thng[cite: 1]
    int flag_F; // -F: thm k hiu phn loi (*, /, @, =, |, %)[cite: 1]
    int flag_f; // -f: tt sp xp[cite: 1]
    int flag_h; // -h: nh dng dung lng d c (K, M, G)[cite: 1]
    int flag_i; // -i: in inode number[cite: 1]
    int flag_k; // -k: hin th theo Kilobytes[cite: 1]
    int flag_l; // -l: long format[cite: 1]
    int flag_n; // -n: numeric UID/GID
    int flag_q; // -q: in '?' cho k t khng in c[cite: 1]
    int flag_R; // -R:  quy[cite: 1]
    int flag_r; // -r: o ngc th t sp xp[cite: 1]
    int flag_S; // -S: sp xp theo kch thc[cite: 1]
    int flag_s; // -s: in s block[cite: 1]
    int flag_t; // -t: sp xp theo thi gian[cite: 2]
    int flag_u; // -u: dng access time (atime)[cite: 2]
    int flag_w; // -w: in raw k t[cite: 2]
} Options;

void parse_options(int argc, char *argv[], Options *opts, int *optind_out);

#endif
