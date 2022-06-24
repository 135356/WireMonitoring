// 跨平台的epoll
// Created by 邦邦 on 2022/6/17.
//
#ifndef BB_S_EPOLL_H
#define BB_S_EPOLL_H
#include <functional>

namespace bb {
    class S_Epoll {
        static const unsigned short EPOLL_MAX_SIZE{2000}; //epoll_fd最大值
        //添加epoll事件(往epoll(内核事件表)里添加事件)
        static bool addEventF_(int &epoll_fd, int &accept_fd, const bool &enable_et=true);
        //从epoll删除事件
        static bool deleteEventF_(int &epoll_fd, int &accept_fd);
    public:
        //epoll使用
        static void runF(int &y_fd,const std::function<bool(int &epoll_fd, int &client_fd)> &f);
    };
}

#endif //BB_S_EPOLL_H
