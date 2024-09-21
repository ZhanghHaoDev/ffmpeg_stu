cmake_minimum_required(VERSION 3.20)
project(ffmpeg-cpp-20)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(__STDC_CONSTANT_MACROS ON)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
get_filename_component(fc_base "fc_base" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(FETCHCONTENT_BASE_DIR ${fc_base})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(glog)
include(gflag)
include(gtest)

add_subdirectory(avformat_model)
add_subdirectory(examples_model)
add_subdirectory(test)