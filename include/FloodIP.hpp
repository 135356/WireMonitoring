//
// Created by 邦邦 on 2022/6/28.
//
#ifndef BB_FLOODIP_H
#define BB_FLOODIP_H
#include "bb/secure/Flood.h"

struct FloodIP{
    bb::secure::Flood a10;
    bb::secure::Flood a20;
    bb::secure::Flood b10;
    bb::secure::Flood b20;
    static FloodIP &obj(){
        static FloodIP bb_secure_flood_id;
        return bb_secure_flood_id;
    }
private:
    FloodIP():a10(3600,10),a20(3600,20),b10(1800,10),b20(1800,20){}
    ~FloodIP()=default;
};

#endif //BB_FLOODIP_H
