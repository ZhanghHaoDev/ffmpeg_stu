include(FetchContent)

FetchContent_Declare(
  boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG master  
)
FetchContent_MakeAvailable(boost)
include_directories(${boost_SOURCE_DIR}/src ${boost_BINARY_DIR})

# 设置 Boost 库的 CMake 选项
set(BOOST_ENABLE_CMAKE ON)
set(BOOST_INCLUDE_LIBRARIES system filesystem)  
