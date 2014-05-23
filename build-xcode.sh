#!/bin/bash -e

set -x

rm -rf _builds/xcode

cmake -H. -B_builds/xcode -DHUNTER_STATUS_DEBUG=ON \
    -DCMAKE_TOOLCHAIN_FILE=${POLLY_ROOT}/xcode.cmake \
    -DSOBER_BUILD_EXAMPLES=ON \
    -DSOBER_BUILD_LONG_TESTS=ON \
    -DSOBER_BUILD_TESTS=ON \
    -GXcode

open _builds/xcode/Sober.xcodeproj
