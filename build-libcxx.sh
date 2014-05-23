#!/bin/bash -e

set -x

rm -rf _builds/libcxx

cmake -H. -B_builds/libcxx -DHUNTER_STATUS_DEBUG=ON \
    -DCMAKE_TOOLCHAIN_FILE=${POLLY_ROOT}/libcxx.cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DSOBER_BUILD_EXAMPLES=ON \
    -DSOBER_BUILD_LONG_TESTS=ON \
    -DSOBER_BUILD_TESTS=ON

cmake --build _builds
