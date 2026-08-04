/*
 * PMA - Package Manager Abstraction
 * the point of this tool is to abstract package managers, reducing the usage
 * down to three basic commands: update, install, remove.
 * meant to be reusable, or compiled as the standalone CLI tool (compile main.c for that.)
 */

#ifndef PMA_H
#define PMA_H

#define PMA_VERSION "0.2"

#include <stdbool.h>

/*
 * main functions
 */

/* just update the system */
int sys_update(void);

/* Install a package */
int package_install(const char *pkg_name, const char *from);

/* Remove a package */
int package_remove(const char *pkg_name, const char *from);

/*
 * utilities
 */

/* determine the system package manager by checking PATH */
const char *determine_package_manager();

/* the system update command for the given package manager (has to be elevated if not running as root) */
const char *get_unelevated_update_command(const char *pkgmgr);

/* build the final update command, potentially elevating it if necessary */
const char *build_final_update_command(const char *pkgmgr);

#ifdef __linux__ /* linux (arch or derivatives) specific */

/* true if an "yay" or "paru" are available */
const bool aur();

/* returns the name of the AUR helper if available, otherwise "none". "yay" or "paru" */
const char *get_aur_helper();

#endif

#ifndef PMA_C
#include "../src/pma.c"
#endif

#endif