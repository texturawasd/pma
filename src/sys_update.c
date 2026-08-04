#include "../common_utils/have.h" // command_exists()
#include "../include/pma.h"
#include "../common_utils/process_utils.h"
#include "../common_utils/simple_strings.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>



int sys_update() {

    const char *system_package_manager = determine_package_manager();

#if defined(__linux__)
    puts("-> Updating the system...");

    /* first check for universal pkgmgrs, snap and flatpak */
    if (command_exists("snap")) {
        puts("-> Updating snap...");
        char *out = NULL, *err = NULL;
        int status = -1;
        const char *update_command_for_snap = build_final_update_command("snap");
        if (run_command_via_exec(update_command_for_snap, &out, &err, &status) == 0) {
            print_command_output(out, err);
            free(out);
            free(err);
            puts("-> updated snap.");
        } else {
            print_command_output(out, err);
            fprintf(stderr, "-> snap update failed with exit status %d\n", status);
            free(out);
            free(err);
        }
    }
    if (command_exists("flatpak")) {
        puts("-> Updating flatpak...");
        char *out = NULL, *err = NULL;
        int status = -1;
        const char *update_command_for_flatpak = build_final_update_command("flatpak");
        if (run_command_via_exec(update_command_for_flatpak, &out, &err, &status) == 0) {
            print_command_output(out, err);
            free(out);
            free(err);
            puts("-> updated flatpak.");
        } else {
            print_command_output(out, err);
            fprintf(stderr, "-> flatpak update failed with exit status %d\n", status);
            free(out);
            free(err);
        }
    }

    /* since Arch is the arch-distro, first check if we're in it or a derivative
     * in order to update with the AUR helper command (simpler) */
    if (aur()) {
        str aur_helper = str_create(get_aur_helper());
        
        printf("-> Detected AUR helper: %s, updating the system\n", aur_helper.data);
        fflush(stdout);
        str_append(&aur_helper, " --noconfirm");
        char *out = NULL, *err = NULL;
        int status = -1;
        if (run_command_via_exec(aur_helper.data, &out, &err, &status) == 0) {
            print_command_output(out, err);
            free(out);
            free(err);
            return 0;
        }

        print_command_output(out, err);
        fprintf(stderr, "-> AUR update failed with exit status %d\n", status);
        free(out);
        free(err);
        return 1;
    }
    /* no aur helper, proceed normally. */
    printf("-> Detected system package manager: %s\n", system_package_manager);

    char *out = NULL, *err = NULL;
    int status = -1;
    const char *final_update_command = build_final_update_command(system_package_manager);
    if (run_command_via_exec(final_update_command, &out, &err, &status) == 0) {
        print_command_output(out, err);
        free(out);
        free(err);
        return 0;
    }

    print_command_output(out, err);
    fprintf(stderr, "-> System update failed with exit status %d\n", status);
    free(out);
    free(err);
    return 1;
#else
    puts("-> Updating the system...");
    printf("-> Detected system package manager: %s\n", system_package_manager);
    char *out = NULL, *err = NULL;
    int status = -1;
    const char *final_update_command = build_final_update_command(system_package_manager);
    if (run_command_via_exec(final_update_command, &out, &err, &status) == 0) {
        print_command_output(out, err);
        free(out);
        free(err);
        return 0;
    }

    print_command_output(out, err);
    fprintf(stderr, "-> System update failed with exit status %d\n", status);
    free(out);
    free(err);
    return 1;
#endif
}
