# Copyright (c) 2014, Ruslan Baratov
# All rights reserved.

cmake_minimum_required(VERSION 3.0)
project(Sober)

option(SOBER_RUN_LONG_TESTS "Run long unit tests" OFF)

include(HunterGate.cmake)
hunter_add_package(Boost COMPONENTS thread system log)
hunter_add_package(CppNetlibUri)
hunter_add_package(Sugar)

include("${SUGAR_ROOT}/cmake/Sugar")
include(sugar_add_gtest)
include(sugar_groups_generate)
include(sugar_include)

# init variables:
#   SOBER_SOURCES
#   SOBER_UT_SOURCES
#   SOBER_LONG_UT_SOURCES
sugar_include("./Source/sober")

list(APPEND SOBER_SOURCES ${SUGAR_SOURCES}) # append used cmake modules

sugar_groups_generate(${SOBER_SOURCES})
sugar_groups_generate(${SOBER_UT_SOURCES})
sugar_groups_generate(${SOBER_LONG_UT_SOURCES})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED thread system log log_setup)
find_package(GTest CONFIG REQUIRED)
find_package(CppNetlibUri CONFIG REQUIRED)

add_library(sober ${SOBER_SOURCES})

add_executable(sober_ut ${SOBER_UT_SOURCES})
target_link_libraries(sober_ut sober GTest::main)

if(SOBER_RUN_LONG_TESTS)
  add_executable(sober_long_ut ${SOBER_LONG_UT_SOURCES})
  target_link_libraries(sober_long_ut sober GTest::main)
endif()

set(INCL_DEST "include")

target_include_directories(
    sober
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/Source>"
    "$<INSTALL_INTERFACE:${INCL_DEST}>"
)

target_compile_definitions(sober PUBLIC BOOST_SPIRIT_USE_PHOENIX_V3)

if(MSVC)
  target_compile_definitions(sober PUBLIC BOOST_ALL_NO_LIB=1)
endif()

# Detect iOS build
string(COMPARE EQUAL "${CMAKE_OSX_SYSROOT}" "iphoneos" is_ios)
if(is_ios)
  # Strange iOS alignment(?) bug (TODO)
  target_compile_definitions(sober PUBLIC BOOST_SP_NO_SYNC)
endif()

target_include_directories(sober SYSTEM PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(sober PUBLIC ${Boost_LIBRARIES} network-uri)

enable_testing()
sugar_add_gtest(SoberUt sober_ut)
if(TARGET sober_long_ut)
  sugar_add_gtest(SoberLongUt sober_long_ut)
endif()
