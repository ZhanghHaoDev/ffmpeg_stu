include(FetchContent)

FetchContent_Declare(
  gflags
  GIT_REPOSITORY https://github.com/gflags/gflags.git
  GIT_TAG master  
)

FetchContent_MakeAvailable(gflags)
include_directories(${gflags_SOURCE_DIR}/src ${gflags_BINARY_DIR})