#!/bin/bash -e

set -x

export XCODE_XCCONFIG_FILE=${POLLY_ROOT}/scripts/NoCodeSign.xcconfig

rm -rf _builds/ios-nocodesign _install/ios-nocodesign

cmake -H. -B_builds/ios-nocodesign -DHUNTER_STATUS_DEBUG=ON \
    -DCMAKE_TOOLCHAIN_FILE=${POLLY_ROOT}/ios-nocodesign.cmake \
    -GXcode \
    -DCMAKE_INSTALL_PREFIX="`pwd`/_install/ios-nocodesign"

cmake --build _builds/ios-nocodesign --config Release --target install
cmake --build _builds/ios-nocodesign --config Debug --target install
