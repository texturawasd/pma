#!/usr/bin/env bash

sources=(
    src/main.c
    common_utils/src/process_utils.c
    common_utils/src/parsing_utils.c
    common_utils/src/elevate.c
    common_utils/src/have.c
    common_utils/src/args.c
    common_utils/src/strings.c
)

cc -o pma "${sources[@]}"