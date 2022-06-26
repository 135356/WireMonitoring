//
// Created by 邦邦 on 2022/6/26.
//

#ifndef WIREMONITORING_TOKEN_H
#define WIREMONITORING_TOKEN_H
#include <set>
#include <openssl/md5.h>
#include <codecvt>
#include "bb/Log.h"

class Token{
    bb::Log log_;
    Token() : log_(1,__FILE__) {
        FILE *fp = fopen("./token.db", "rb");
        if (!fp) {return;}
        std::array<uint64_t,2> md5{};
        for(;fread(&md5,16,1,fp) == 1;){
            token.insert(md5);
        }
        fclose(fp);
    }
    ~Token(){
        if(token.empty()){return ;}
        FILE *fp = fopen("./token.db", "wb");
        if (!fp) {
            log_.error("./token.db,文件打开失败");
            return;
        }
        size_t size = token.size();
        if(size > 100000){
            log_.warn("token > 100000,已自动清理了token");
            return ;
        }
        if(fwrite(&token,16,size,fp) != size){
            log_.error("./token.db,文件写入错误");
        }
        fclose(fp);
    };
public:
    static std::string UnicodeToUTF8(const std::wstring &wstr){
        std::string ret;
        try {
            std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
            ret = wcv.to_bytes(wstr);
        } catch (const std::exception & e) {
            std::cerr << e.what() << std::endl;
        }
        return ret;
    }

    static std::wstring UTF8ToUnicode(const std::string &str){
        std::wstring ret;
        try {
            std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
            ret = wcv.from_bytes(str);
        } catch (const std::exception & e) {
            std::cerr << e.what() << std::endl;
        }
        return ret;
    }

    std::set<std::array<uint64_t,2>> token{};
    static Token &obj(){
        static Token user_token;
        return user_token;
    }
};

#endif //WIREMONITORING_TOKEN_H
