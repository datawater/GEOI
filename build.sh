#! /usr/bin/env bash

set -xe

g++ -pedantic -DLOCAL -std=gnu++20 $1 -o $1.out