#! /usr/bin/env bash

set -xe

gcc -Wall -Wextra -shared -fPIC -O3 -o malloc_override.so malloc_override.c -ldl