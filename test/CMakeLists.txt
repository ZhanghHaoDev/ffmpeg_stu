include_directories(
    "../avformat_model/"
    "../examples_model"
)

enable_testing()

add_executable(test_executable
    avformat_model_test.cc
    examples_model_test.cc
)

target_link_libraries(test_executable PUBLIC 
    glog::glog
    gtest_main
    avformat_model
    examples_model
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavformat.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavcodec.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavutil.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libswresample.dylib"
)

include(GoogleTest) 
gtest_discover_tests(test_executable) 