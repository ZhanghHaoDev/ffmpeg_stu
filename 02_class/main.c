#include <stdio.h>
#include <libavformat/avformat.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <input_flv_file> <output_aac_file>\n", argv[0]);
        return -1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    AVFormatContext *input_format_context = NULL;
    AVFormatContext *output_format_context = NULL;
    const AVOutputFormat *output_format = NULL;
    AVPacket packet;
    // 打开输入文件
    if (avformat_open_input(&input_format_context, input_filename, NULL, NULL) < 0) {
        fprintf(stderr, "Could not open input file '%s'\n", input_filename);
        return -1;
    }

    // 获取输入文件的流信息
    if (avformat_find_stream_info(input_format_context, NULL) < 0) {
        fprintf(stderr, "Could not find stream information\n");
        return -1;
    }

    // 打开输出文件
    avformat_alloc_output_context2(&output_format_context, NULL, NULL, output_filename);
    if (!output_format_context) {
        fprintf(stderr, "Could not create output context\n");
        return -1;
    }

    output_format = output_format_context->oformat;

    // 查找音频流
    int audio_stream_index = -1;
    for (unsigned int i = 0; i < input_format_context->nb_streams; i++) {
        if (input_format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }

    if (audio_stream_index == -1) {
        fprintf(stderr, "Could not find audio stream in the input file\n");
        return -1;
    }

    // 创建输出音频流
    AVStream *input_stream = input_format_context->streams[audio_stream_index];
    AVStream *output_stream = avformat_new_stream(output_format_context, NULL);
    if (!output_stream) {
        fprintf(stderr, "Failed allocating output stream\n");
        return -1;
    }

    if (avcodec_parameters_copy(output_stream->codecpar, input_stream->codecpar) < 0) {
        fprintf(stderr, "Failed to copy codec parameters\n");
        return -1;
    }

    output_stream->codecpar->codec_tag = 0;

    // 打开输出文件
    if (!(output_format->flags & AVFMT_NOFILE)) {
        if (avio_open(&output_format_context->pb, output_filename, AVIO_FLAG_WRITE) < 0) {
            fprintf(stderr, "Could not open output file '%s'\n", output_filename);
            return -1;
        }
    }

    // 写文件头
    if (avformat_write_header(output_format_context, NULL) < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        return -1;
    }

    // 读取输入文件的音频数据并写入输出文件
    while (av_read_frame(input_format_context, &packet) >= 0) {
        if (packet.stream_index == audio_stream_index) {
            packet.stream_index = output_stream->index;
            if (av_interleaved_write_frame(output_format_context, &packet) < 0) {
                fprintf(stderr, "Error muxing packet\n");
                break;
            }
        }
        av_packet_unref(&packet);
    }

    // 写文件尾
    av_write_trailer(output_format_context);

    // 关闭输入和输出文件
    avformat_close_input(&input_format_context);
    if (output_format_context && !(output_format->flags & AVFMT_NOFILE)) {
        avio_closep(&output_format_context->pb);
    }
    avformat_free_context(output_format_context);

    printf("Audio extracted successfully to '%s'\n", output_filename);
    return 0;
}