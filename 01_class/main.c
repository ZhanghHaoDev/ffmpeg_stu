#include <stdio.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>

int main() {
    // 输出 FFmpeg 版本信息
    printf("FFmpeg version: %s\n", av_version_info());

    return 0;
}