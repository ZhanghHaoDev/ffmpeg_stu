add_library(avformat_model STATIC
    avformat_model.cc
)

target_compile_definitions(avformat_model PRIVATE __STDC_CONSTANT_MACROS)

target_link_libraries(avformat_model PUBLIC 
    glog::glog
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavformat.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavcodec.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libavutil.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2_1/lib/libswresample.dylib"
)