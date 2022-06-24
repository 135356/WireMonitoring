// 防洪
// Created by 邦邦 on 2022/6/17.
//
#ifndef BB_FLOOD_H
#define BB_FLOOD_H
#include <string>
#include <map>
#include "bb/Time.h"

namespace bb {
    class Flood {
        //time/60 //距离现在的分钟数
        //time/3600 //距离现在的小时数60*60
        //time/86400 //距离现在的天数60*60*24
        bool is_one_rm{};
        bool is_two_rm{};
        //保存用户的ip与访问次数，
        std::map<uint32_t, int> one;
        std::map<uint32_t, int> two;
        uint32_t interval_seconds_{}; //间隔时间内
        uint32_t max_number_{}; //可以触发的最大次数
    public:
        Flood() = default;

        explicit Flood(const uint32_t &interval_seconds = 3600, const uint32_t &max_number = 60);

        //验证ip是否超过访问次数
        bool isIp(uint32_t &s_addr);
    };
}

#endif //BB_FLOOD_H
