//
// Created by 邦邦 on 2022/4/20.
//
#ifndef MYSQLORM_DEBUG_H
#define MYSQLORM_DEBUG_H
#include <iostream>
#include <string>
#include <utility>
#include "bb/Time.h"
#include "bb/Log.h"

namespace bb {
    class Debug:public Log{
    public:
        explicit Debug(std::string file_path={}): Log(1,std::move(file_path)){}
    };
}

#endif //MYSQLORM_DEBUG_H