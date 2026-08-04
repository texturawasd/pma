#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "../common_utils/elevate.h" // elevate()
#include "../common_utils/have.h"    // command_exists()

/* determine the package manager(s) available. */
const char *determine_package_manager() {

    char *pkgmgr = NULL;

#if defined(__FreeBSD__)
    /* FreeBSD */
    pkgmgr = "pkg";

#elif defined(__OpenBSD__)
    /* OpenBSD */
    pkgmgr = "pkg_add";
#elif defined(__APPLE__)
    /* macOS */
    pkgmgr = "softwareupdate";
#elif defined(__linux__)
    /* linux distros */
    if (command_exists("pacman")) {
        pkgmgr = "pacman";
    } else if (command_exists("apt-get") || command_exists("apt")) {
        pkgmgr = "apt";
    } else if (command_exists("dnf")) {
        pkgmgr = "dnf";
    } else if (command_exists("yum")) {
        pkgmgr = "dnf"; // yum is link to dnf
    } else if (command_exists("zypper")) {
        pkgmgr = "zypper";
    } else if (command_exists("apk")) {
        pkgmgr = "apk";
    } else if (command_exists("xbps-install")) {
        pkgmgr = "xbps";
    } else if (command_exists("emaint") || command_exists("emerge")) {
        pkgmgr = "emerge";
    } else if (command_exists("eopkg") == 1) {
        pkgmgr = "eopkg";
    } else if (command_exists("nix-channel") || command_exists("nixos-rebuild")) {
        pkgmgr = "nix";
    } else if (command_exists("guix")) {
        pkgmgr = "guix";
    } else if (command_exists("urpmi")) {
        pkgmgr = "urpmi";
    } else if (command_exists("swupd")) {
        pkgmgr = "swupd";
    } else {
        pkgmgr = NULL;
    }
#endif
    return pkgmgr;
}

// Returns the (not yet elevated) update command for the given pkgmgr (you may need to insert sudo)
const char *get_unelevated_update_command(const char *pkgmgr) {

    const char *unelevated_update_command = NULL;

    /* linux system pkgmgrs */
#if defined(__linux__)
    if (strcmp(pkgmgr, "pacman") == 0) {
        unelevated_update_command = "pacman -Syu --noconfirm";
    } else if (strcmp(pkgmgr, "apt") == 0) {
        unelevated_update_command = "apt-get update && apt-get dist-upgrade -y";
    } else if (strcmp(pkgmgr, "dnf") == 0) {
        unelevated_update_command = "dnf upgrade -y";
    } else if (strcmp(pkgmgr, "zypper") == 0) {
        unelevated_update_command = "zypper ref && zypper dup -y";
    } else if (strcmp(pkgmgr, "apk") == 0) {
        unelevated_update_command = "apk update && apk upgrade";
    } else if (strcmp(pkgmgr, "xbps") == 0) {
        unelevated_update_command = "xbps-install -Syu";
    } else if (strcmp(pkgmgr, "emerge") == 0) {
        unelevated_update_command = "emaint sync -a && emerge --update --deep --with-bdeps=y @world";
    } else if (strcmp(pkgmgr, "eopkg") == 0) {
        unelevated_update_command = "eopkg up -y";
    } else if (strcmp(pkgmgr, "nix") == 0) {
        unelevated_update_command = "nix-channel --update && nixos-rebuild";
    } else if (strcmp(pkgmgr, "guix") == 0) {
        unelevated_update_command = "guix pull && guix system reconfigure";
    } else if (strcmp(pkgmgr, "urpmi") == 0) {
        unelevated_update_command = "urpmi.update -a && urpmi --auto-select --auto";
    } else if (strcmp(pkgmgr, "swupd") == 0) {
        unelevated_update_command = "swupd update";
    }
#elif defined(__FreeBSD__)
    unelevated_update_command = "freebsd-update fetch install && pkg update && pkg upgrade -y";
#elif defined(__OpenBSD__)
    unelevated_update_command = "syspatch && pkg_add -u";
#elif defined(__APPLE__)
    unelevated_update_command = "softwareupdate --install --all";
#endif
    return unelevated_update_command;
}

static inline const bool am_i_in_arch_or_a_derivative_or_otherwise_is_pacman_available() {
    if (strcmp(determine_package_manager(), "pacman") == 0) {
        return true; /* pacman is available so i must be */
    }
    return false;
}

const bool aur() {
    if (am_i_in_arch_or_a_derivative_or_otherwise_is_pacman_available()) {
        // not gonna aur-help, check for available aur helpers
        if (command_exists("yay") || command_exists("paru")) {
            return true;
        }
    }
    return false;
}

const char *get_aur_helper() {
    const char *AUR_HELPER = "none";
    if (command_exists("yay")) {
        AUR_HELPER = "yay";
    } else if (command_exists("paru")) {
        AUR_HELPER = "paru";
    }
    return AUR_HELPER;
}

// Returns the (not yet elevated) install command for the given pkgmgr (you may need to insert sudo)
const char *get_unelevated_install_command(const char *pkgmgr) {

    const char *unelevated_install_command = NULL;

    // first handle flatpak and snap
    if (strcmp(pkgmgr, "flatpak") == 0) {
        unelevated_install_command = "flatpak install";
    } else if (strcmp(pkgmgr, "snap") == 0) {
        unelevated_install_command = "snap install";
    }

    /* linux system pkgmgrs */
#if defined(__linux__)
    if (strcmp(pkgmgr, "pacman") == 0) {
        unelevated_install_command = "pacman -S --needed --noconfirm";
    } else if (strcmp(pkgmgr, "apt") == 0) {
        unelevated_install_command = "apt-get install -y";
    } else if (strcmp(pkgmgr, "dnf") == 0) {
        unelevated_install_command = "dnf install -y";
    } else if (strcmp(pkgmgr, "zypper") == 0) {
        unelevated_install_command = "zypper install -y";
    } else if (strcmp(pkgmgr, "apk") == 0) {
        unelevated_install_command = "apk add";
    } else if (strcmp(pkgmgr, "xbps") == 0) {
        unelevated_install_command = "xbps-install -y";
    } else if (strcmp(pkgmgr, "emerge") == 0) {
        unelevated_install_command = "emerge";
    } else if (strcmp(pkgmgr, "eopkg") == 0) {
        unelevated_install_command = "eopkg install -y";
    } else if (strcmp(pkgmgr, "nix") == 0) {
        unelevated_install_command = "nix-env -iA";
    } else if (strcmp(pkgmgr, "guix") == 0) {
        unelevated_install_command = "guix install";
    } else if (strcmp(pkgmgr, "urpmi") == 0) {
        unelevated_install_command = "urpmi --auto";
    } else if (strcmp(pkgmgr, "swupd") == 0) {
        unelevated_install_command = "swupd bundle-add";
    }
#elif defined(__FreeBSD__)
    unelevated_install_command = "pkg install";
#elif defined(__OpenBSD__)
    unelevated_install_command = "pkg_add ";
#elif defined(__APPLE__)
    unelevated_install_command = "I don't know";
#endif
    return unelevated_install_command;
}

// Returns the (not yet elevated) install command for the given pkgmgr (you may need to insert sudo)
const char *get_unelevated_remove_command(const char *pkgmgr) {

    const char *unelevated_remove_command = NULL;

    // first handle flatpak and snap
    if (strcmp(pkgmgr, "flatpak") == 0) {
        unelevated_remove_command = "flatpak install";
    } else if (strcmp(pkgmgr, "snap") == 0) {
        unelevated_remove_command = "snap install";
    }

    /* linux system pkgmgrs */
#if defined(__linux__)
    if (strcmp(pkgmgr, "pacman") == 0) {
        unelevated_remove_command = "pacman -Rns --noconfirm";
    } else if (strcmp(pkgmgr, "apt") == 0) {
        unelevated_remove_command = "apt-get remove -y";
    } else if (strcmp(pkgmgr, "dnf") == 0) {
        unelevated_remove_command = "dnf remove -y";
    } else if (strcmp(pkgmgr, "zypper") == 0) {
        unelevated_remove_command = "zypper remove -y";
    } else if (strcmp(pkgmgr, "apk") == 0) {
        unelevated_remove_command = "apk del";
    } else if (strcmp(pkgmgr, "xbps") == 0) {
        unelevated_remove_command = "xbps-remove -y";
    } else if (strcmp(pkgmgr, "emerge") == 0) {
        unelevated_remove_command = "emerge --deselect";
    } else if (strcmp(pkgmgr, "eopkg") == 0) {
        unelevated_remove_command = "eopkg remove -y";
    } else if (strcmp(pkgmgr, "nix") == 0) {
        unelevated_remove_command = "nix-env -e";
    } else if (strcmp(pkgmgr, "guix") == 0) {
        unelevated_remove_command = "guix remove";
    } else if (strcmp(pkgmgr, "urpmi") == 0) {
        unelevated_remove_command = "urpme --auto";
    } else if (strcmp(pkgmgr, "swupd") == 0) {
        unelevated_remove_command = "swupd bundle-remove";
    }
#elif defined(__FreeBSD__)
    unelevated_install_command = "pkg remove";
#elif defined(__OpenBSD__)
    unelevated_install_command = "pkg_delete";
#elif defined(__APPLE__)
    unelevated_install_command = "I don't know";
#endif
    return unelevated_remove_command;
}
