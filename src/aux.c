#include "../common_utils/args.h"
#include "../include/pma.h"
#include <stdio.h>
#include <stdlib.h>

static const char *_help = "h,help,version,ver,v";

void do_args(int argc, char **argv) {
    if (arg_is_in_list(argv[1], _help)) {
        printf("pma " PMA_VERSION " - texturawasd\n"
               "Compiled on %s at %s\n",
               __DATE__, __TIME__ "\nCommands:\n"
                                  "\tupdate - update the system"

        );
        exit(0);
    }
}