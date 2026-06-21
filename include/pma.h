#ifndef PMA_H
#define PMA_H

const char *determine_package_manager();

const char *get_unelevated_update_command(const char *pkgmgr);

const char *build_final_update_command(const char *pkgmgr);

const bool aur();

const char *get_aur_helper();

#if 0
const char *package_install_command(const char *pkgmgr, const char *package_name);
const char *package_remove_command(const char *pkgmgr, const char *package_name);
#endif

#ifndef PMA_C
#include "../src/pma.c"
#endif

#endif