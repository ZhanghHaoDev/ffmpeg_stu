#include "avformat_model.h"

int main(int argc, char* argv[]){
    avformat_model avformat_model;

    avformat_model.demuxing("test.mp4");

    return 0;
}