#include <stdio.h>
#include "options.h"
#include "entry.h"

int main(int argc, char *argv[]) {
    Options opts = {0};
    int optind_out = 1;

    parse_options(argc, argv, &opts, &optind_out);

    int num_paths = argc - optind_out;

    if (num_paths == 0) {
        process_path(".", &opts, 0);
    } else if (num_paths == 1) {
        process_path(argv[optind_out], &opts, 0);
    } else {
        for (int i = optind_out; i < argc; i++) {
            if (i > optind_out) printf("\n");
            process_path(argv[i], &opts, 1);
        }
    }

    return 0;
}
