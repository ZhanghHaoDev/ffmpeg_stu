#ifndef EXAMPLES_MODEL_H
#define EXAMPLES_MODEL_H

#include <string>
#include <vector>

class examples_model{
public:
    examples_model();
    ~examples_model();

    const char *type_string(int type);
    int avio_list_dir(std::string file_path);

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

    /**
    * @brief 解析MP4文件，提取并打印文件的基本信息
    * 
    * @param file_path MP4文件路径
    * @return int 错误码
    */
    int parse_mp4_file(std::string file_path); 

    /**
    * @brief 获取音频文件的采样率
    * 
    * @param input_file 文件路径
    * @return 文件的采样值数组
    */
    template<typename T> 
    std::vector<T> get_audio_sample_rate(std::string input_file);

    /**
     * @brief 解码音频文件
     * 
     * @param file_path 文件路径
     * @return int 错误码
     */
    int decode_audio(std::string file_path);

    /**
     * @brief 解码视频文件
     * 
     * @param file_path 文件路径
     * @return int 错误码
     */
    int decode_video(std::string file_path);

     /**
     * @brief 编码音频数据
     * 
     * @param input_file 输入的音频文件路径
     * @param output_file 输出的编码文件路径
     * @return int 错误码
     */
    int encode_audio(std::string input_file, std::string output_file);

    /**
     * @brief 编码视频数据
     * 
     * @param input_file 输入的视频文件路径
     * @param output_file 输出的编码文件路径
     * @return int 错误码
     */
    int encode_video(std::string input_file, std::string output_file);

    /**
     * @brief 提取视频当中的音频
     * 
     * @param input_file 输入的视频文件
     * @param output_file 输出的音频文件
     * @return int 
     */
    int video_extract_audio(std::string input_file,std::string output_file);

private:


};

#endif // EXAMPLES_MODEL_H