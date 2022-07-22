#ifndef BB_WORK_H
#define BB_WORK_H
#include <map>
#include <string>
#include <sys/wait.h>
#include "bb/http/Serve.h"
#include "api/Route.h"

namespace bb{
    class Work{
        Work()=default;
        ~Work()=default;
        unsigned short THREAD_MAX{100}; //最大监听线程数
        //监听80端口
        void run_(int port=80);
        //记录pid
        static void forkWrite_(unsigned pid=getpid());
        static void forkWrite_(unsigned pid,unsigned pid1);
        //信号通信
        static void forkStop_(int signum);
    public:
        //测试模式
        static void test();
        //正式模式
        static void run();
        //杀死进程
        static void forkKill();
    };
}

#endif