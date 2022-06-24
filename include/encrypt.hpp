//
// Created by 邦邦 on 2022/6/24.
//

#ifndef WIREMONITORING_ENCRYPT_HPP
#define WIREMONITORING_ENCRYPT_HPP
#include <string>
#include <openssl/md5.h>

struct encrypt{
    static void md5(const std::string &src,std::string &dst){
        //定义md5存储长度
        unsigned char s_md5[MD5_DIGEST_LENGTH];
        //调用OPENSSL md5加密函数
        MD5((unsigned char*)&src, src.size(), (unsigned char*)&s_md5);
        dst.resize(33);
        for(int i = 0; i < 16; i++){
            //将加密字符串写入mdString变量中
            sprintf(&dst[i*2], "%02x", (unsigned int)s_md5[i]);
        }
    }
};

#endif //WIREMONITORING_ENCRYPT_HPP
