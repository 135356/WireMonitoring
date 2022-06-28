//
// Created by 邦邦 on 2022/6/28.
//
#ifndef BB_FLOODIP_H
#define BB_FLOODIP_H
#include "bb/Flood.h"

struct FloodIP{
    bb::Flood a10000;
    static FloodIP &obj(){
        static FloodIP bb_flood_id;
        return bb_flood_id;
    }
private:
    FloodIP():a10000(3600,10000){}
    ~FloodIP()=default;
};

#endif //BB_FLOODIP_H
