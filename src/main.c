/*
 * sys_update
 * a tool to update the system in any system
 * texturawasd, june 2026
 * version 0.1: supports linux and FreeBSD
 * technically others too, but only tested on:
 * Arch Linux
 * Ubuntu
 * FreeBSD
*/

#include <stdio.h>
#include <stdbool.h>
#include "sys_update.c"
#include "../common_utils/args.h"
#include "../common_utils/parsing_utils.h"

int main(int argc, char **argv) {

    if (arg_is_present("update", argc, argv)) {
        sys_update();
        return 0;
    } else if (arg_is_present("install", argc, argv)) {
        puts("installing packages is not yet implemented, exiting.");
        return -1;
    } else if (arg_is_present("remove", argc, argv)) {
        puts("removing packages is not yet implemented, exiting.");
        return -1;
    }

    /* no args, update the system */
    puts("confirm to update the system? (y/n)");
    char response[4];
    scanf("%s", response);
    if (parse_bool(response)) {
        sys_update();
    } else {
        puts("aborting.");
    }
    return 0;

}

