//
// Created by 邦邦 on 2022/6/24.
//

#ifndef WIREMONITORING_ENCRYPT_HPP
#define WIREMONITORING_ENCRYPT_HPP
#include <string>
#include <openssl/md5.h>

struct encrypt{
    static void md5(const char src[],char dst[32]){
        unsigned char sMD5[16] = {0};
        unsigned char* ret = MD5((const unsigned char*)src, strlen(src), sMD5);
        for(int i = 0; i < 16; i++){
            //将加密字符串写入dst变量中
            sprintf(&dst[i*2], "%02x",sMD5[i]);
        }

        /*MD5_CTX ctx;
        MD5_Init(&ctx);
        for (const char &v : src){
            MD5_Update(&ctx, (void*)&v, 1);
        }
        unsigned char sMD5[16] = {0};
        int ret = MD5_Final(sMD5, &ctx);
        //printf("ret:%d \n", ret);
        for(int i = 0; i < 16; i++){
            //将加密字符串写入dst变量中
            sprintf(&dst[i*2], "%02x",s_md5[i]);
        }*/
    }
};

#endif //WIREMONITORING_ENCRYPT_HPP
