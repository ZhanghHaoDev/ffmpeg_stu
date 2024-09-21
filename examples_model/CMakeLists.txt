add_library(examples_model STATIC
    examples_model.cc
)

target_compile_definitions(examples_model PRIVATE __STDC_CONSTANT_MACROS)

target_link_libraries(examples_model PUBLIC 
    glog::glog
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavformat.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavcodec.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavutil.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libswresample.dylib"
)