#!/usr/bin/env sh

cc -o pma src/main.c \
    common_utils/src/parsing_utils.c \
    common_utils/src/have.c \
    common_utils/src/args.c \
    common_utils/src/strings.c