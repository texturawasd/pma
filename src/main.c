/*
 * pma - Package Manager Abstraction
 * a tool to abstract package managers to a single tool in any system
 * texturawasd, august 2026
 * version 0.2: supports linux and FreeBSD
 * technically others too, but only tested on:
 * Arch Linux
 * Alpine Linux
 * Ubuntu
 * FreeBSD
 */

#include "../common_utils/args.h"
#include "../common_utils/parsing_utils.h"
#include "aux.c"
#include "sys_ins_rem.c"
#include "sys_update.c"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

/* Be lenient to idiots who may forget the commands of their package manager, such as myself */
static const char *_update_list = "up,Up,update,Update,upgrade,Upgrade";
static const char *_install_list = "add,Add,install,Install";
static const char *_remove_list = "rem,Rem,remove,Remove,del,Del,delete,Delete,uninstall,Uninstall,R";

int main(int argc, char **argv) {

    do_args(argc, &*argv);

    /* I'd rather not be ran as root */
    if (getuid() == 0 && !arg_is_present("i-know-what-i-am-doing", argc, argv)) {
        fprintf(stderr, "Running %s as root is not recommended. Pass --i-know-what-i-am-doing if you really do.\n", argv[0]);
        return -1;
    }

    /* System update */
    if (arg_is_in_list(argv[1], _update_list)) {
        goto update;
        return 0;

        /* Install a package*/
    } else if (arg_is_in_list(argv[1], _install_list)) {
        if (argc < 3) {
            fprintf(stderr,
                    "Too few arguments. To install a package, pass a package name");
            return -1;
        }

        package_install(argv[2], "system");
        return 0;

        /* Remove a package */
    } else if (arg_is_in_list(argv[1], _remove_list)) {
        if (argc < 3) {
            fprintf(stderr,
                    "Too few arguments. To install a package, pass a package name");
            return -1;
        }
        package_remove(argv[2], "system");
        return 0;
    }

    /* no args, update the system */
    puts("confirm to update the system? (y/n)");
    char response[420];
    scanf("%s", response);
    if (parse_bool(response) == true) {
        goto update;
    } else {
        puts("aborting.");
        exit(1);
    }
update:
    sys_update();
    return 0;
}
