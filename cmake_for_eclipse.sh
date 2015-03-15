#!/bin/sh

rm -rf CMakeCache.txt CMakeFiles/ CMakeScripts/ CTestTestfile.cmake cmake_install.cmake || exit 1
cmake -G "Eclipse CDT4 - Unix Makefiles"  -D_ECLIPSE_VERSION=3.7

exit 0
