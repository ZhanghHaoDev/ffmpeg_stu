#include "avformat_model.h"

#include <string>

#include "gtest/gtest.h"

TEST(avformat_model,demuxing_test){
    avformat_model avformat_model;
    EXPECT_EQ(avformat_model.demuxing("test.mp4"),0);
}