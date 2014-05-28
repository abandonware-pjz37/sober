#!/bin/bash -e

set -x

rm -rf _builds/xcode _install/xcode

cmake -H. -B_builds/xcode -DHUNTER_STATUS_DEBUG=ON \
    -DCMAKE_TOOLCHAIN_FILE=${POLLY_ROOT}/xcode.cmake \
    -DSOBER_RUN_LONG_TESTS=ON \
    -GXcode \
    -DCMAKE_INSTALL_PREFIX="`pwd`/_install/xcode"

cmake --build _builds/xcode --config Release --target install
cmake --build _builds/xcode --config Debug --target install
