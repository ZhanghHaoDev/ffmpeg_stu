#ifndef EXAMPLES_MODEL_H
#define EXAMPLES_MODEL_H

#include <string>

class examples_model{
public:
    examples_model();
    ~examples_model();

    /**
     * @brief 输出指定目录下的文件信息
     * 
     * @param dir 指定目录
     * @return int 错误码
     */
    int list_dir(std::string dir);

    /**
     * @brief 打印流媒体的参数信息
     * 
     * @param file_path 文件路径
     * @return int 错误码
     */
    int print_info(std::string file_path);

    /**
     * @brief 音频文件转换，任意的音频格式转换为pcm
     * 
     * @param audio_file 输入的音频文件
     * @param pcm_file 转换后的pcm数据 
     * @return int 
     */
    int audio_encode_pcm(std::string audio_file,std::string pcm_file);

private:


};

#endif // EXAMPLES_MODEL_H