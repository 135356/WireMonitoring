// http协议
// Created by 邦邦 on 2022/6/17.
//
#ifndef BB_HTTP_H
#define BB_HTTP_H
#include <map>
#include <string>
#include <fcntl.h> //O_RDONLY
#include <sys/stat.h> //fstat
#include "bb/tcp/IpTcp.h"
#include "bb/tcp/S_Epoll.h"
#include "bb/tcp/AnalyseHttp.h"
#include "api/HttpRoutePath.hpp"

namespace bb{
    class Http{
        static const unsigned short THREAD_MAX{1}; //最大监听线程数
        static const unsigned short SEND_BUF_MAX{1472}; //每次最大发送字节
        static const unsigned short RECF_MAX_SIZE{65535}; //每次最大接收的字节
        //文件对应的类型(Content-Type)
        std::map<std::string,std::string> content_type_{
                {"html","text/html;charset=utf8"},
                {"htm","text/html;charset=utf8"},
                {"jsp","text/html;charset=utf8"},
                {"txt","text/plain;charset=utf8"},
                {"css","text/css;charset=utf8"},
                {"ico","application/x-icon"},
                {"js","application/javascript;charset=utf8"},
                {"map","application/javascript;charset=utf8"}, //vue的js debug文件
                {"xml","application/xml;charset=utf8"},
                {"json","application/json;charset=utf8"},
                {"jpg","image/jpeg"},
                {"jpe","image/jpeg"},
                {"jpeg","image/jpeg"},
                {"gif","image/gif"},
                {"png","image/png"},
                {"mp3","audio/mp3"},
                {"mp4","video/mpeg4"},
                {"mpeg","video/mpg"},
                {"default","text/html;charset=utf8"}
        };
        //要响应的http头
        void sendHeadF_(std::string &s_head,bb::AnalyseHttp *http_info,const std::string &type_key,const size_t &file_size={},const std::string &state="200 OK"){
            s_head = "HTTP/1.1 "+state+"\r\n"
                "Content-Type: "+content_type_[type_key]+"\r\n"
                "Access-Control-Allow-Method: "+http_info->method+"\r\n" //GET,POST,OPTIONS,PUT,DELETE,PATCH,HEAD(发PUT请求前会先发OPTIONS请求进行预检)
                "Access-Control-Allow-Headers: x-requested-with,content-type\r\n"; //x-requested-with,content-type包含Access-Control-Request-Headers附带的内容//http_info->head_data["Access-Control-Request-Headers"]
            //如果使用epoll必须指定文件长度，否则会一直等待，超时才会断开链接，浏览器才会去发下一个文件
            if(file_size){s_head+="Content-Length: "+std::to_string(file_size)+"\r\n";}
            /*if(!http_info->head_data["Origin"].empty()){
                "Access-Control-Allow-Origin: "+http_info->head_data["Origin"]+"\r\n" //等于*或Origin附带的内容(cors,如果请求中附带有Origin表示有跨域验证)
                "Access-Control-Allow-Credentials: true\r\n"; //允许客户端携带验证信息(例如 cookie 之类的,Access-Control-Allow-Origin: *时不能为true)
            }*/
            s_head += "\r\n";
        }
        //处理http请求
        int handleHttpF_(int &socket_fd, const char r_buf[], size_t &r_length) {
            //std::cout<<r_buf<<std::endl;
            //分析出来的http头信息
            auto *http_info = new bb::AnalyseHttp(r_buf);
            std::string s_head{};
            std::string s_data{};
            size_t s_size{}; //要发送的内容长度
            auto processRequestFF = [&]() -> ssize_t {
                if (HttpRoutePath::obj().route.count(http_info->path)) { //路由请求
                    if (http_info->method == "GET") {
                        HttpRoutePath::obj().route[http_info->path](http_info->get_map, s_data, s_size); //运行路由里面的方法，传递接收到的数据 与 发送的数据(&别名)
                    } else if (http_info->method == "POST") {
                        std::map<std::string, std::string> r_data;
                        http_info->getBody(r_data); //分析获取到的post数据
                        HttpRoutePath::obj().route[http_info->path](r_data, s_data, s_size);
                    }

                    sendHeadF_(s_head, http_info, "html", s_size);
                    if (send(socket_fd, s_head.c_str(), s_head.size(), 0) == -1) { return -1; }
                    if (send(socket_fd, s_data.c_str(), s_data.size(), 0) == -1) { return -1; }
                } else { //文件 或 其它请求
                    if (http_info->method == "GET") {
                        std::string file_type; //文件类型
                        std::string file_path = "./html/wire_monitoring" + http_info->path;
                        //获取文件属性
                        struct stat file_stat{};
                        int ret = stat(file_path.c_str(), &file_stat);
                        if (ret == -1) { //如果这个文件 或 目录不存在
                            sendHeadF_(s_head, http_info, "html", 0, "400 Not file(文件不存在)");
                            return send(socket_fd, s_head.c_str(), s_head.size(), 0);
                        }
                        //是目录
                        if (S_ISDIR(file_stat.st_mode)) {
                            file_type = "html";
                            file_path = "./html/wire_monitoring" + http_info->path + "/index.html";
                            stat(file_path.c_str(), &file_stat);
                        } else {
                            file_type = http_info->path.substr(http_info->path.rfind('.') + 1);
                        }
                        //打开文件
                        FILE *file_fp = fopen(file_path.c_str(), "rb");
                        if (!file_fp) {
                            sendHeadF_(s_head, http_info, "html", 0, "400 Not file(文件不存在)");
                            return send(socket_fd, s_head.c_str(), s_head.size(), 0);
                        }
                        //找到了文件准备发送
                        if (!content_type_.count(file_type)) { //请求的文件类型是否配置content_type
                            sendHeadF_(s_head, http_info, "html", 0, "405 content-type Not allowed(文件类型不支持)");
                            return send(socket_fd, s_head.c_str(), s_head.size(), 0);
                        }
                        //响应头
                        sendHeadF_(s_head, http_info, file_type, file_stat.st_size, "200 OK");
                        if (send(socket_fd, s_head.c_str(), s_head.size(), 0) < 0) { return -1; }
                        //内容响应
                        while (true) {
                            char s_buf[SEND_BUF_MAX]{};
                            size_t s_f_size = fread(&s_buf[0], 1, SEND_BUF_MAX, file_fp);
                            if (s_f_size > 0) {
                                if (send(socket_fd, s_buf, s_f_size, 0) == -1) {
                                    printf("续传失败\n");
                                    fclose(file_fp);
                                    return -2;
                                }
                            } else {
                                fclose(file_fp);
                                return 0;
                            }
                        }
                    } else if (http_info->method == "OPTIONS") { //响应一个确让
                        sendHeadF_(s_head, http_info, "html", 0);
                        return send(socket_fd, s_head.c_str(), s_head.size(), 0);
                    } else { //请求的路由地址不存在
                        sendHeadF_(s_head, http_info, "html", 0, "501 Not Implemented(路由地址不存在)");
                        return send(socket_fd, s_head.c_str(), s_head.size(), 0);
                    }
                }
                return 0;
            };
            int state = (int) processRequestFF();
            delete http_info;
            return state;
        }
    public:
        explicit Http(int port=80){
            bb::IpTcp tcp({}, port);
            bb::S_Epoll::runF(tcp.socket_,[this](int &epoll_fd, int &client_fd){
                char r_buf[RECF_MAX_SIZE] = {};
                size_t r_length = recv(client_fd, r_buf, RECF_MAX_SIZE, 0);
                if (r_length > 0) {
                    handleHttpF_(client_fd,r_buf,r_length);
                    return true;
                } else {
                    return false;
                }
            });
            /*bb::IpTcp tcp({}, port); //网络通信协议tcp/ip
            std::vector<std::thread> thread_arr{};
            thread_arr.reserve(THREAD_MAX);
            for(int i=0;i<THREAD_MAX;i++){
                thread_arr.emplace_back([&](){
                    bb::S_Epoll::runF(tcp.socket_,[this](int &epoll_fd, int &client_fd){
                        char r_buf[RECF_MAX_SIZE] = {};
                        size_t r_length = recv(client_fd, r_buf, RECF_MAX_SIZE, 0);
                        if (r_length > 0) {
                            handleHttpF_(client_fd,r_buf,r_length);
                            return true;
                        } else {
                            return false;
                        }
                    });
                });
            }
            for(auto &v:thread_arr){
                if(v.joinable()){v.join();}
            }*/
        }
    };
}

#endif //BB_HTTP_H
