#include "Work.h"

namespace bb{
    Work::Work(){
        signal(SIGABRT,stopF_); //当该进程收到停止信号时触发stopF_函数
    }
    Work::~Work()=default;
    void Work::runF_(int port){
        bb::S_Epoll epoll(port,10);
        epoll.runF([](int &client_fd,unsigned &client_ip)->bool{
            http::Serve *http_serve = new http::Serve(client_fd);
            //printf("aaa:%s\n",http_serve->r_buf);
            auto routeF = [&]()->bool{
                //如果http没有解析出有用数据就直接返回
                if(!http_serve->info){return false;}
                //要发送的内容、长度
                std::string s_data{};size_t s_size{};
                if(http_serve->info->head_map["content-type"] == "application/json"){ //路由请求
                    if (Route::obj().route.count(http_serve->info->path)) {
                        if (http_serve->info->method == "GET"){
                            Route::obj().route[http_serve->info->path](client_ip,http_serve->info->get_map,s_data,s_size); //运行路由里面的方法，传递接收到的数据 与 发送的数据(&别名)
                            return http_serve->sendF("get",s_data,s_size);
                        }else if (http_serve->info->method == "POST") {
                            //获取post数据
                            std::map<std::string, std::string> r_data;
                            if(http_serve->recvJson(r_data)){
                                Route::obj().route[http_serve->info->path](client_ip,r_data,s_data, s_size);
                                return http_serve->sendF("post",s_data,s_size);
                            }else{
                                return http_serve->sendF("get","400 type or Content-length error");
                            }
                        } else if (http_serve->info->method == "OPTIONS") {
                            return http_serve->sendF("options","200 ok");
                        }else{
                            return http_serve->sendF("get","400 Not method");
                        }
                    }else{
                        return http_serve->sendF("get","404 Not route");
                    }
                }else if(http_serve->info->head_map["content-type"] == "multipart/form-data"){ //用户端上传文件
                    if (Route::obj().route_upload.count(http_serve->info->path)) {
                        if(Route::accessTokenVerification(client_ip,http_serve->info->get_map["access_token"])){
                            //保存在服务器的文件名称(根据用户名保存的，所以用户名不允许修改)
                            if(http_serve->recvFile(http_serve->info->get_map["access_token"])){
                                Route::obj().route_upload[http_serve->info->path](http_serve->info->get_map,s_data,s_size);
                                return http_serve->sendF("get",s_data,s_size); //通知客户端上传结果
                            }else{
                                return http_serve->sendF("get","500 error");
                            }
                        }else{
                            return http_serve->sendF("get","200 token error");
                        }
                    }else{
                        return http_serve->sendF("get","404 Not route");
                    }
                }else{
                    if(http_serve->info->get_map["request_type"] == "download"){ //客户端下载文件
                        if(Route::accessTokenVerification(client_ip,http_serve->info->get_map["access_token"])){
                            return http_serve->sendFile(http_serve->info->get_map["access_token"]);
                        }else{
                            return http_serve->sendF("get","500 error");
                        }
                    }else if (http_serve->info->method == "GET") { //网页 或 其它请求
                        return http_serve->sendHtml();
                    }else{
                        return http_serve->sendF("get","404 Not path");
                    }
                }
            };
            bool is = routeF();
            delete http_serve;
            return is;
        });
    }
    void Work::testF(){
        runF_();
    }
    void Work::formalF(){
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
            waitpid(c_pid, nullptr, 0);
        }
    }
    void Work::stopF(char path[]){
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
}