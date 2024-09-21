#include "examples_model.h"

#include <string>

#include "gtest/gtest.h"

std::string list_dir_file = "/Users/zhanghao/code/cpp/ffmpeg_stu/build";
std::string print_info_file_1 = "/Users/zhanghao/code/cpp/ffmpeg_stu/test/test_file/1227469313-1-16.mp4";
std::string print_info_file_2 = "/Users/zhanghao/code/cpp/ffmpeg_stu/test/test_file/output_audio.mp3";
std::string audio_encode_pcm_file = "/Users/zhanghao/code/cpp/ffmpeg_stu/test/test_file/output_audio.pcm";

TEST(examples_model,list_dir_test){
    examples_model examples;
    EXPECT_EQ(examples.list_dir(list_dir_file),0);
}

TEST(examples_model,print_info_test){
    examples_model examples;
    EXPECT_EQ(examples.print_info(print_info_file_1),0);
    EXPECT_EQ(examples.print_info(print_info_file_2),0);
}

TEST(examples_model,audio_encode_pcm_test){
    examples_model examples;
    EXPECT_EQ(examples.audio_encode_pcm(print_info_file_2, audio_encode_pcm_file),0);
}