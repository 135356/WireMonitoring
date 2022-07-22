//
// Created by 邦邦 on 2022/6/28.
#ifndef WIREMONITORING_WMFLOOD_H
#define WIREMONITORING_WMFLOOD_H
#include "bb/Flood.h"

struct WMFlood{
    bb::Flood a10;
    bb::Flood b10;
    static WMFlood &obj(){
        static WMFlood wm_flood;
        return wm_flood;
    }
private:
    WMFlood():a10(3600,10),b10(3600,10){}
    ~WMFlood()=default;
};

#endif