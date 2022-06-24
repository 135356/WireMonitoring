//
// Created by 邦邦 on 2022/4/22.
//

#ifndef BB_LOG_H
#define BB_LOG_H
#include <string>
#include <exception>
#include "Time.h"

namespace bb {
    class Log{
        short mode_; //0=不写日记、1=写日记
        std::string msg_arr_; //消息内容
        std::string file_path_{}; //发生异常的文件路径与名称
    public:
        explicit Log(const short &mode=1,std::string file_path={});
        ~Log();
    public:
        //修改模式
        void setMode(short mode);
        //一般信息
        void info(std::string msg);
        //警告信息
        void warn(std::string msg);
        //同上，添加错误行
        void warn(std::string msg,int line);
        //错误信息
        void error(std::string msg);
        //同上，添加错误行
        void error(std::string msg,int line);
    };
}
#endif //BB_LOG_H
