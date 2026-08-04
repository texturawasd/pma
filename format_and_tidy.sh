#!/usr/bin/env sh

clang-format -i src/*.c

clang-tidy -fix src/*.c

clang-format -i include/*.h

clang-tidy -fix include/*.h