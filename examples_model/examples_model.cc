#include "examples_model.h"

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <fstream>
#include <sys/stat.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#include "glog/logging.h"

extern "C"{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavformat/avio.h"
    #include "libavutil/file.h"
    #include "libavutil/mem.h"
    #include "libavutil/opt.h"
    #include "libavutil/channel_layout.h"
    #include "libavutil/samplefmt.h"
    #include "libavutil/timestamp.h"
    #include "libswresample/swresample.h"
}

examples_model::examples_model(){
    google::InitGoogleLogging("examples_model");
    google::SetStderrLogging(google::INFO); 
    google::InstallFailureSignalHandler();  
}

examples_model::~examples_model(){
    google::ShutdownGoogleLogging();
}

int examples_model::list_dir(std::string dir){
    int error_code = 0;
    if(dir.empty() != 0){
        LOG(ERROR) << "输入路径为空";
        return error_code = -1;
    }
    std::cout << std::left << std::setw(9) << "文件类型" << std::setw(12) << "文件大小"
              << std::setw(30) << "文件名称" << std::setw(10) << "用户和组"
              << std::setw(10) << "文件模式" << std::setw(20) << "修改时间"
              << std::setw(20) << "访问时间"<< std::setw(20) << "状态更改时间"
              << std::endl;

    for (const auto entry : std::filesystem::directory_iterator(dir)){
        std::string type = entry.is_directory() ? "<DIR>" : "<FILE>";
        auto size = entry.is_regular_file() ? entry.file_size() : 0;
        auto name = entry.path().filename().string();
        struct stat file_stat;
        if (stat(entry.path().c_str(),&file_stat) != 0){
            perror("stat");
            continue;
        }
        struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group *gr = getgrgid(file_stat.st_gid);
        std::string uid_and_gid = pw ? pw->pw_name : "N/A";
        uid_and_gid += "(";
        uid_and_gid += gr ? gr->gr_name : "N/A";
        uid_and_gid += ")";

        std::string filemode = std::to_string(file_stat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
        auto mod_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(file_stat.st_mtime));
        std::string mod_time_str = std::asctime(std::localtime(&mod_time));
        mod_time_str.pop_back(); 
        auto acc_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(file_stat.st_atime));
        std::string acc_time_str = std::asctime(std::localtime(&acc_time));
        acc_time_str.pop_back(); 
        auto status_change_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::from_time_t(file_stat.st_ctime));
        std::string status_change_time_str = std::asctime(std::localtime(&status_change_time));
        status_change_time_str.pop_back(); 
        
        std::cout << std::left << std::setw(9) << type << std::setw(12) << size
                  << std::setw(30) << name << std::setw(10) << uid_and_gid
                  << std::setw(10) << filemode << std::setw(20) << mod_time_str
                  << std::setw(20) << acc_time_str << std::setw(20) << status_change_time_str
                  << std::endl;
    }

    return error_code;
}

int examples_model::print_info(std::string file_path){
    int error_code = 0;
    AVFormatContext *fmt_ctx = nullptr;
    if ((error_code =avformat_open_input(&fmt_ctx, file_path.c_str(), nullptr, nullptr)) < 0){
        LOG(ERROR) << "获取文件头部信息错误" << file_path;
        return error_code;
    }

    if ((error_code = avformat_find_stream_info(fmt_ctx, nullptr)) < 0){
        LOG(ERROR) << "获取文件流信息错误" << file_path;
        return error_code;
    }

    LOG(INFO) << "文件名称：" << file_path;
    LOG(INFO) << "文件格式信息：" << fmt_ctx->iformat->name;

    AVDictionaryEntry *tag = nullptr;
    while ((tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        LOG(INFO) << "元数据: " << tag->key << " = " << tag->value << std::endl;
    }
    LOG(INFO) << "文件时长：" << fmt_ctx->duration / AV_TIME_BASE << " 秒";
    LOG(INFO) << "文件比特率：" << fmt_ctx->bit_rate << " bps";

    for(int i = 0; i < fmt_ctx->nb_streams; i++){
        AVStream *stream = fmt_ctx->streams[i];
        AVCodecParameters *codecper = stream->codecpar;

        std::string codec_type;
        if (codecper->codec_type == AVMEDIA_TYPE_VIDEO){
            codec_type = "Video";
        }else if (codecper->codec_type == AVMEDIA_TYPE_AUDIO){
            codec_type = "Audio";
        } else {
            codec_type = "Other";
        }
        LOG(INFO) << "stream " << i << " type : " << codec_type;

        const AVCodec *codec = avcodec_find_decoder(codecper->codec_id);
        if (codec){
            LOG(INFO) << "codec " << codec->name << "\t" << codec->long_name;
        }

        if (codecper->codec_type == AVMEDIA_TYPE_VIDEO){
            LOG(INFO) << "分辨率: " << codecper->width << "x " << codecper->height;
            LOG(INFO) << "通道数: " << codecper->ch_layout.nb_channels;
        }

        if(codecper->codec_type == AVMEDIA_TYPE_AUDIO){
            LOG(INFO) << "采样率: " << codecper->sample_rate << "Hz";
            LOG(INFO) << "通道数: " << codecper->ch_layout.nb_channels;
        }
    }
    avformat_close_input(&fmt_ctx);

    return error_code;  
}

// 输出 SwrContext 的参数
void log_swr_ctx_params(SwrContext *swr_ctx) {
    char val_str[128];
    int64_t val_int;
    int ret;

    // 输出输入和输出的采样格式
    ret = av_opt_get(swr_ctx, "in_sample_fmt", 0, (uint8_t **)&val_int);
    if (ret >= 0) {
        LOG(INFO) << "输入采样格式: " << av_get_sample_fmt_name((AVSampleFormat)val_int);
    }

    ret = av_opt_get(swr_ctx, "out_sample_fmt", 0, (uint8_t **)&val_int);
    if (ret >= 0) {
        LOG(INFO) << "输出采样格式: " << av_get_sample_fmt_name((AVSampleFormat)val_int);
    }

    // 输出输入和输出的采样率
    ret = av_opt_get(swr_ctx, "in_sample_rate", 0, (uint8_t **)&val_int);
    if (ret >= 0) {
        LOG(INFO) << "输入采样率: " << val_int;
    }

    ret = av_opt_get(swr_ctx, "out_sample_rate", 0, (uint8_t **)&val_int);
    if (ret >= 0) {
        LOG(INFO) << "输出采样率: " << val_int;
    }

    // 输出输入和输出的通道布局
    ret = av_opt_get(swr_ctx, "in_channel_layout", 0, (uint8_t **)&val_int);
    if (ret >= 0) {
        LOG(INFO) << "输入通道布局: " << val_int;
    }

    ret = av_opt_get(swr_ctx, "out_channel_layout", 0, (uint8_t **)&val_int);
    if (ret >= 0) {
        LOG(INFO) << "输出通道布局: " << val_int;
    }
}

int examples_model::audio_encode_pcm(std::string audio_file,std::string pcm_file){
    int error_code = 0;
    if(audio_file.empty() != 0 || pcm_file.empty() != 0){
        LOG(ERROR) << "文件为空";
        return error_code = -1;
    }

    AVFormatContext *fmt_ctx = nullptr;
    if ((error_code = avformat_open_input(&fmt_ctx,audio_file.c_str(),nullptr,nullptr)) < 0){
        LOG(ERROR) << "打开文件头部错误";
        return error_code;
    }

    if ((error_code = avformat_find_stream_info(fmt_ctx, nullptr)) < 0){
        LOG(ERROR) << "获取文件信息错误";
        return error_code;
    }

    const AVCodec *codec = nullptr;
    AVCodecContext *codec_ctx = nullptr;
    int audio_stream_index = -1;
    for (unsigned int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            codec = avcodec_find_decoder(fmt_ctx->streams[i]->codecpar->codec_id);
            if (!codec) {
                LOG(ERROR) << "找不到解码器";
                avformat_close_input(&fmt_ctx);
                return error_code = -1;
            }
            codec_ctx = avcodec_alloc_context3(codec);
            if (!codec_ctx) {
                LOG(ERROR) << "无法分配解码器上下文";
                avformat_close_input(&fmt_ctx);
                return error_code = -1;
            }
            if ((error_code = avcodec_parameters_to_context(codec_ctx, fmt_ctx->streams[i]->codecpar)) < 0) {
                LOG(ERROR) << "无法复制解码器参数到解码器上下文";
                avcodec_free_context(&codec_ctx);
                avformat_close_input(&fmt_ctx);
                return error_code;
            }
            if ((error_code = avcodec_open2(codec_ctx, codec, nullptr)) < 0) {
                LOG(ERROR) << "无法打开解码器";
                avcodec_free_context(&codec_ctx);
                avformat_close_input(&fmt_ctx);
                return error_code;
            }
            break;
        }
    }

    if (audio_stream_index == -1) {
        LOG(ERROR) << "找不到音频流";
        avformat_close_input(&fmt_ctx);
        return error_code = -1;
    }

    std::ofstream pcm_out(pcm_file, std::ios::binary);
    if (!pcm_out.is_open()) {
        LOG(ERROR) << "无法打开输出文件";
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return error_code = -1;
    }
    SwrContext *swr_ctx = nullptr;
    AVChannelLayout out_ch_layout = codec_ctx->ch_layout;
    AVChannelLayout in_ch_layout = codec_ctx->ch_layout;
    if ((error_code = swr_alloc_set_opts2(
            &swr_ctx,
            &out_ch_layout,       // 输出通道布局
            AV_SAMPLE_FMT_FLTP,    // 输出采样格式
            codec_ctx->sample_rate, // 输出采样率
            &in_ch_layout,        // 输入通道布局
            AV_SAMPLE_FMT_FLTP,   // 输入采样格式
            codec_ctx->sample_rate, // 输入采样率
            0,                    // 日志偏移量
            nullptr               // 日志上下文
        )) < 0) {
        LOG(ERROR) << "无法初始化重采样上下文";
        return error_code;
    }

    if ((error_code = swr_init(swr_ctx)) > 0){
        LOG(INFO) << "swr_init 初始化错误";
        return error_code;
    }

    AVPacket *out_pack = av_packet_alloc();
    AVFrame *frname = av_frame_alloc();
    AVFrame *swr_frname = av_frame_alloc();
    swr_frname->format = AV_SAMPLE_FMT_S16;
    swr_frname->ch_layout.nb_channels = codec_ctx->ch_layout.nb_channels;
    swr_frname->sample_rate = codec_ctx->sample_rate;
    
    while(av_read_frame(fmt_ctx,out_pack) >= 0){
        if(out_pack->stream_index == audio_stream_index){
            while((error_code = avcodec_send_packet(codec_ctx, out_pack)) == AVERROR(EAGAIN)){
                while((error_code = avcodec_receive_frame(codec_ctx,frname)) >= 0){
                    swr_frname->nb_samples = frname->nb_samples;
                    swr_frname->format = AV_SAMPLE_FMT_FLTP;
                    swr_frname->ch_layout.nb_channels = codec_ctx->ch_layout.nb_channels;
                    swr_frname->sample_rate = codec_ctx->sample_rate;
                    LOG(INFO) << swr_frname->format;
                    LOG(INFO) << frname->format;

                    if ((error_code = av_frame_get_buffer(swr_frname, 0)) < 0) {
                        LOG(ERROR) << "无法分配重采样帧缓冲区";
                        break;
                    }
                    if ((error_code = swr_convert_frame(swr_ctx, swr_frname, frname)) < 0) {
                        char errbuf[128];
                        av_strerror(error_code, errbuf, sizeof(errbuf));
                        LOG(ERROR) << "重采样失败: " << errbuf;
                        break;
                    }




                }
            }    
        }
    }





    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);
    pcm_out.close();

    return error_code;
}
