#! /usr/bin/env bash

set -xe

g++ -Wall -Wextra -Wshadow -DLOCAL -std=gnu++11 -O2 -pipe -g $1 -o $1.exe