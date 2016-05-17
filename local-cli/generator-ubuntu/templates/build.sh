#!/bin/bash

# XXX: Don't move this script
cd $(dirname $0)

# Workaround
rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake Makefile

# Build project
cmake . && make && cp ./bin/<%= name %> click/


