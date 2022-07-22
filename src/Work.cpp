#include "Work.h"

namespace bb{
    void Work::run_(int port){
        http::Serve::runF(port,[](http::Serve &http)->bool{
            //printf("%s\n",http.r_buf);
            std::string s_data{};size_t s_size{}; //要发送的内容、长度
            if(http.info->head_map["content-type"] == "application/json"){ //路由请求
                if (Route::obj().route.count(http.info->path)) {
                    if (http.info->method == "get"){
                        Route::obj().route[http.info->path](http.client_ip,http.info->get_map,s_data,s_size); //运行路由里面的方法，传递接收到的数据 与 发送的数据(&别名)
                        return http.sendF("get",s_data,s_size);
                    }else if (http.info->method == "post") {
                        //获取post数据
                        std::map<std::string, std::string> r_data;
                        http.info->getBody(r_data);

                        Route::obj().route[http.info->path](http.client_ip,r_data,s_data, s_size);
                        return http.sendF("post",s_data,s_size);
                    } else if (http.info->method == "options") {
                        return http.sendF("options","200 ok");
                    }else{
                        return http.sendF("get","400 Not method");
                    }
                }else{
                    return http.sendF("get","404 Not route");
                }
            }else if(http.info->head_map["content-type"] == "multipart/form-data"){ //用户端上传文件
                if (Route::obj().route_upload.count(http.info->path)) {
                    if(Route::accessTokenVerification(http.client_ip,http.info->get_map["access_token"])){
                        std::string file_saved_name; //保存在服务器的文件名称(根据用户名保存的，所以用户名不允许修改)
                        if(http.recvFile(http.info->get_map["access_token"],file_saved_name)){
                            http.info->get_map["file_saved_name"] = file_saved_name;
                            Route::obj().route_upload[http.info->path](http.info->get_map,s_data,s_size);
                            return http.sendF("get",s_data,s_size); //通知客户上传结果
                        }else{
                            return http.sendF("get","500 error");
                        }
                    }else{
                        return http.sendF("get","200 token error");
                    }
                }else{
                    return http.sendF("get","404 Not route");
                }
            }else{
                //http://127.0.0.1/1658392442?request_type=download&access_token=6884723583339005016471758643470247414_12771166362151970525166324445869724780
                if(http.info->get_map["request_type"] == "download"){ //用户端下载文件
                    if(Route::accessTokenVerification(http.client_ip,http.info->get_map["access_token"])){
                        return http.sendFile(http.info->get_map["access_token"],http.info->path);
                    }else{
                        return http.sendF("get","500 error");
                    }
                }else if (http.info->method == "get") { //网页 或 其它请求
                    return http.sendHtml();
                }else{
                    return http.sendF("get","404 Not path");
                }
            }
        });
    }
    void Work::forkWrite_(unsigned pid){
        FILE *fp = fopen("./WM_pid.key", "wb");
        if(fwrite(&pid,4,1,fp) != 1){
            bb::Log::obj().error("./WM_pid.key 写入错误");
        }
        fclose(fp);
    }
    void Work::forkWrite_(unsigned pid,unsigned pid1){
        FILE *fp = fopen("./WM_pid.key", "wb");
        if(fwrite(&pid,4,1,fp) != 1){
            bb::Log::obj().error("./WM_pid.key 写入错误");
        }
        if(fwrite(&pid1,4,1,fp) != 1){
            bb::Log::obj().error("./WM_pid.key 写入错误");
        }
        fclose(fp);
    }
    void Work::forkStop_(int signum){
        if(signum == SIGABRT){ //终止程序
            exit(-1);
        }
    }
    void Work::test(){
        forkWrite_();
        Work work;
        work.run_();
    }
    void Work::run(){
        unsigned pid=getpid();
        while(true){
            pid_t c_pid = fork();
            if(c_pid == 0){
                forkWrite_(pid,getpid());
                Work work;
                signal(SIGABRT,forkStop_); //信号通信
                std::vector<std::thread> thread_arr{};
                thread_arr.reserve(work.THREAD_MAX);
                for(int i=0;i<work.THREAD_MAX;i++){
                    thread_arr.emplace_back([&](){
                        work.run_();
                    });
                }
                for(auto &v:thread_arr){
                    if(v.joinable()){v.join();}
                }
            }
            waitpid(c_pid, nullptr, 0);
        }
    }
    void Work::forkKill(){
        FILE *fp = fopen("./WM_pid.key", "rb");
        if(!fp){perror("程序没有启动");return ;}
        int pid{},pid1{};
        if(fread(&pid,4,1,fp) != 1){
            perror("./WM_pid.key 读取失败");
        }else if(fread(&pid1,4,1,fp) != 1){}
        if(pid1){ //先停止子进程
            kill(pid1 ,SIGABRT);
        }
        if(pid){ //在停止首进程
            kill(pid ,SIGABRT);
        }
        fclose(fp);
    }
}