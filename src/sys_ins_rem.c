// UNFINISHED

#include "../common_utils/simple_strings.h"
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "../common_utils/process_utils.h"
#include "../include/pma.h"

const char *get_unelevated_install_command(const char *pkgmgr) {
    //TODO implement
}

int package_install(const char *pkg_name, const char *from) {

    if (!pkg_name || !from) { return -1; }

    if (strcmp(from, "flatpak") == 0) {
        /* Install pkg from flatpak */
    } else if (strcmp(from, "snap") == 0) {
        /* Install pkg from snap */
    } else if (strcmp(from, "system") == 0) {
        /* Install using system packange manager*/
        str install_command = str_create(get_unelevated_install_command(determine_package_manager()));
        str_append(&install_command, pkg_name);
        str_prepend(&install_command, determine_elevator());

        char *out = NULL, *err = NULL; int status = -1;
        if (run_command_via_exec(install_command.data, &out, &err, &status) == 0) {
            print_command_output(out, err);
            free(out);
            free(err);
            printf("-> Installed %s\n", pkg_name);
        }

    } else {
        fprintf(stderr,
        "Invalid 'from'. Can be 'flatpak', 'snap', or 'system'."
        );
        return -1;
    }

    const char *pkgmgr = determine_package_manager();

}


int package_remove(const char *pkg_name) {

}