#ifndef BB_WORK_H
#define BB_WORK_H
#include <map>
#include <string>
#include <sys/wait.h>
#include "bb/http/Config.hpp"
#include "bb/http/Analyze.h"
#include "bb/http/Serve.h"
#include "api/Route.h"

namespace bb{
    class Work{
        Work();
        ~Work();
        unsigned short THREAD_MAX{100}; //最大监听线程数
        void runF_(int port=80); //监听80端口
        static void stopF_(int signum){
            if(signum == SIGABRT){ //判断接收到的信号
                exit(0);
            }
        }
    public:
        //测试模式
        void testF();
        //正式模式
        void formalF();
        //停止进程
        void stopF(char path[]);
        static Work &obj(){
            static Work bb_work;
            return bb_work;
        }
    };
}

#endif