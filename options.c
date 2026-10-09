#include <unistd.h>
#include "options.h"

void parse_options(int argc, char *argv[], Options *opts, int *optind_out) {
    int opt;
    
    // Thit lp mc nh cho -q v -w da trn output l Terminal hay khng[cite: 1, 2]
    if (isatty(STDOUT_FILENO)) {
        opts->flag_q = 1;
        opts->flag_w = 0;
    } else {
        opts->flag_q = 0;
        opts->flag_w = 1;
    }

    // c tt c cc c theo ng chui synopsis trong ls.pdf: AacdFfhiklnqRrSstuw[cite: 1]
    while ((opt = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (opt) {
            case 'A': opts->flag_A = 1; break;
            case 'a': opts->flag_a = 1; break;
            case 'C': opts->flag_C = 1; opts->flag_u = 0; break; // -C  -u[cite: 1, 2]
            case 'd': opts->flag_d = 1; opts->flag_R = 0; break; // -d  -R[cite: 1, 2]
            case 'F': opts->flag_F = 1; break;
            case 'f': opts->flag_f = 1; break;
            case 'h': opts->flag_h = 1; opts->flag_k = 0; break; // -h  -k[cite: 1]
            case 'i': opts->flag_i = 1; break;
            case 'k': opts->flag_k = 1; opts->flag_h = 0; break; // -k  -h[cite: 1]
            case 'l': opts->flag_l = 1; opts->flag_n = 0; break; // -l  -n[cite: 2]
            case 'n': opts->flag_n = 1; opts->flag_l = 0; break; // -n  -l[cite: 2]
            case 'q': opts->flag_q = 1; opts->flag_w = 0; break; // -q  -w[cite: 2]
            case 'R': if (!opts->flag_d) opts->flag_R = 1; break;
            case 'r': opts->flag_r = 1; break;
            case 'S': opts->flag_S = 1; break;
            case 's': opts->flag_s = 1; break;
            case 't': opts->flag_t = 1; break;
            case 'u': opts->flag_u = 1; opts->flag_C = 0; break; // -u  -C[cite: 2]
            case 'w': opts->flag_w = 1; opts->flag_q = 0; break; // -w  -q[cite: 2]
            default: break;
        }
    }
    *optind_out = optind;
}
