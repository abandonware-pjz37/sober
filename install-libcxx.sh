#!/bin/bash -e

set -x

rm -rf _builds/libcxx _install/libcxx

cmake -H. -B_builds/libcxx -DHUNTER_STATUS_DEBUG=ON \
    -DCMAKE_TOOLCHAIN_FILE=${POLLY_ROOT}/libcxx.cmake \
    -DSOBER_RUN_LONG_TESTS=ON \
    -GXcode \
    -DCMAKE_INSTALL_PREFIX="`pwd`/_install/libcxx"

cmake --build _builds/libcxx --config Release --target install
cmake --build _builds/libcxx --config Debug --target install
