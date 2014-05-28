#!/bin/bash -e

set -x

rm -rf _builds/ios _install/ios

cmake -H. -B_builds/ios -DHUNTER_STATUS_DEBUG=ON \
    -DCMAKE_TOOLCHAIN_FILE=${POLLY_ROOT}/ios.cmake \
    -GXcode \
    -DCMAKE_INSTALL_PREFIX="`pwd`/_install/ios"

cmake --build _builds/ios --config Release --target install
cmake --build _builds/ios --config Debug --target install
