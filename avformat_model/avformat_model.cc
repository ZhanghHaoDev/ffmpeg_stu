#include "avformat_model.h"

extern "C"{
    #include "libavutil/avutil.h"
    #include "libavformat/avformat.h"
}
#include "glog/logging.h"

avformat_model::avformat_model(){
    av_log_set_level(AV_LOG_QUIET);
    google::InitGoogleLogging("avformat_model");
    google::SetStderrLogging(google::INFO); 
    google::InstallFailureSignalHandler();  
}

avformat_model::~avformat_model(){
    google::ShutdownGoogleLogging();
}

int avformat_model::demuxing(std::string input_file){
    int error_code = 0;

    return error_code;
}

