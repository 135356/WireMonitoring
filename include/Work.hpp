#ifndef BB_WORK_H
#define BB_WORK_H
#include <map>
#include <string>
#include <sys/wait.h>
#include "bb/net/http/Config.hpp"
#include "bb/net/http/Serve.h"
#include "Route.h"

class Work{
    Work(){
        signal(SIGABRT,stopF_); //当该进程收到停止信号时触发stopF_函数
    }
    ~Work()=default;
    unsigned short THREAD_MAX{100}; //最大监听线程数
    //监听80端口
    void runF_(int port=80){
        bb::net::TcpEpoll epoll(port,6);
        epoll.runF([](int &client_fd,unsigned &client_ip)->bool{
            auto routeF = [](unsigned &client_ip,bb::net::http::Serve *http_serve)->bool{
                std::string s_data{}; //要发送的内容
                size_t s_size{}; //要发送的长度
                if (http_serve->info->method == "GET"){
                    if (Route::obj().route.count(http_serve->info->path)) { //路由请求
                        Route::obj().route[http_serve->info->path](client_ip,http_serve->info->get_map,s_data,s_size); //运行路由里面的方法，传递接收到的数据 与 发送的数据(&别名)
                        return http_serve->sendF("get",s_data,s_size);
                    }else{
                        if(http_serve->info->get_map["request_type"] == "download"){ //文件下载请求
                            return http_serve->sendFile();
                        }else{ //网页 或 其它请求
                            return http_serve->sendHtml();
                        }
                    }
                }else if (http_serve->info->method == "POST") {
                    if (Route::obj().route.count(http_serve->info->path)) {
                        if(http_serve->info->head_map["content-type"] == "application/json"){
                            //获取post数据
                            std::map<std::string, std::string> r_data;
                            if(http_serve->recvJson(r_data)){
                                Route::obj().route[http_serve->info->path](client_ip,r_data,s_data, s_size);
                                return http_serve->sendF("post",s_data,s_size);
                            }
                        }
                        return http_serve->sendF("get","400 (content-length is application/json) or (content-length error)");
                    }else if (Route::obj().route_upload.count(http_serve->info->path)) {
                        if(http_serve->info->head_map["content-type"] == "multipart/form-data"){ //用户端上传文件
                            if(Route::accessTokenVerification(client_ip,http_serve->info->get_map["access_token"])){
                                //保存在服务器的文件名称(根据用户名保存的，所以用户名不允许修改)
                                if(http_serve->recvFile(http_serve->info->get_map["access_token"])){
                                    Route::obj().route_upload[http_serve->info->path](http_serve->info->get_map,s_data,s_size);
                                    return http_serve->sendF("get",s_data,s_size); //通知客户端上传结果
                                }
                                return http_serve->sendF("get","500 error");
                            }else{
                                return http_serve->sendF("get","200 token error");
                            }
                        }
                        return http_serve->sendF("get","400 (content-length is application/json) or (content-length error)");
                    }
                    return http_serve->sendF("get","404 Not POST route");
                } else if (http_serve->info->method == "OPTIONS") {
                    return http_serve->sendF("options","200 ok");
                }else{
                    return http_serve->sendF("get","400 Not method");
                }
            };

            bb::net::http::Serve *http_serve = new bb::net::http::Serve(client_fd);
            //printf("r_buf:%s\n",http_serve->r_buf);
            if(!http_serve->info){return false;} //如果http没有解析出有用数据就直接返回
            bool is = routeF(client_ip,http_serve);
            delete http_serve;
            return is;
        });
    };
    static void stopF_(int signum){
        if(signum == SIGABRT){ //判断接收到的信号
            exit(0);
        }
    }
public:
    //测试模式
    void testF(){
        runF_();
    }
    //正式模式
    void formalF(){
        while(true){
            pid_t c_pid = fork();
            if(c_pid == 0){
                std::vector<std::thread> thread_arr{};
                thread_arr.reserve(THREAD_MAX);
                for(int i=0;i<THREAD_MAX;i++){
                    thread_arr.emplace_back([&](){
                        runF_();
                    });
                }
                for(auto &v:thread_arr){
                    if(v.joinable()){v.join();}
                }
            }
            bb::secure::Log::obj().info("退出子进程");
            waitpid(c_pid, nullptr, 0);
        }
    }
    //停止进程
    void stopF(char path[]){
        size_t start_i,end_i=strlen(path);
        for(start_i=end_i;start_i>0;start_i--){
            if(path[start_i] == '/'){
                start_i++;break;
            }
        }
        char name[end_i-start_i];
        memmove(&name[0],&path[start_i],end_i-start_i);

        char cmd[end_i-start_i+11];
        sprintf(cmd,"killall -%d %s",SIGABRT,name);
        cmd[end_i-start_i+11]='\0';

        system(cmd);
    }
    static Work &obj(){
        static Work bb_work;
        return bb_work;
    }
};

#endif