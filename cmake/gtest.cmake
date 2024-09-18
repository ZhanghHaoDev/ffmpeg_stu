include(FetchContent)

FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main  
)

FetchContent_MakeAvailable(gtest)
include_directories(${gtest_SOURCE_DIR}/src ${gtest_BINARY_DIR})