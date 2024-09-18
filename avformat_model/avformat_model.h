#ifndef AVFORMAT_MODEL_H
#define AVFORMAT_MODEL_H

#include <string>

class avformat_model{
public:
    avformat_model();
    ~avformat_model();

    int demuxing(std::string input_file);

private:

};

#endif // AVFORMAT_MODEL_H