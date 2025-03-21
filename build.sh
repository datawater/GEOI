#! /usr/bin/env bash

set -xe

g++ -pedantic -DLOCAL -std=gnu++20 -g $1 -o $1.exe