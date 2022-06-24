// 分析http头信息
// Created by 邦邦 on 2022/6/17.
//
#ifndef BB_ANALYSEHTTP_H
#define BB_ANALYSEHTTP_H
#include <string>
#include <map>
#include <utility>

namespace bb {
    //解析http请求
    class AnalyseHttp {
        //构造时调用，解析第一行数据得到method、path、treaty，然后调用setGetMap_得到get传参的数据
        void first_(std::string &head_first);
        //first_调用，得到get传参的数据
        void setGetMap_(uint32_t &path_end_i);
    public:
        size_t start_index{}; //数据内容在字符串中的开始位置，数据如果是数组用body_arr_index标记下标
        bool body_is_big_brackets{},body_is_small_brackets{}; //数据是大括号开始还是小括号开始的
        std::string r_http_head_; //接收到的原始http头数据
        std::string method,path,treaty; //GET,/a1_get?aaa=123&bbb=bbb,HTTP/1.1
        std::map<std::string, std::string> head_map; //头信息
        std::map<std::string, std::string> get_map; //get传递的数据
        //对数据进行分割
        explicit AnalyseHttp(std::string r_http_head);
        //外部调用得到post传递的参数
        bool getBody(std::map<std::string,std::string> &body_map)const;
    };
}

#endif //BB_ANALYSEHTTP_H
