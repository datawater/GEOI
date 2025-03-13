#! /usr/bin/env bash

set -xe

g++ -fsanitize=address -fsanitize=undefined -fsplit-stack -pedantic -DLOCAL -std=gnu++20 $1 -o $1.out