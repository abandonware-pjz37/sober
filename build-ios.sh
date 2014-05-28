#!/bin/bash -e

set -x

rm -rf _builds/ios

cmake -H. -B_builds/ios -DHUNTER_STATUS_DEBUG=ON \
    -DCMAKE_TOOLCHAIN_FILE=${POLLY_ROOT}/ios.cmake \
    -DSOBER_BUILD_TESTS=ON \
    -DSOBER_BUILD_LONG_TESTS=ON \
    -DSOBER_BUILD_EXAMPLES=ON \
    -GXcode

open _builds/ios/Sober.xcodeproj
