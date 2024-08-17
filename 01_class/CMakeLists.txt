add_executable(demo
    main.c
)

target_link_libraries(demo PUBLIC 
    ${FFMPEG_LIB_DIR}/libavcodec.a
    ${FFMPEG_LIB_DIR}/libavdevice.a
    ${FFMPEG_LIB_DIR}/libavfilter.a
    ${FFMPEG_LIB_DIR}/libavformat.a
    ${FFMPEG_LIB_DIR}/libavutil.a
    ${FFMPEG_LIB_DIR}/libswresample.a
    ${FFMPEG_LIB_DIR}/libswscale.a

    z
    iconv
    "-framework VideoToolbox"
    "-framework CoreVideo"
    "-framework CoreMedia"
    "-framework Security"
    "-framework AudioToolbox"
    "-lbz2"
    "-framework CoreFoundation"
)