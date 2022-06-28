//
// Created by 邦邦 on 2022/6/26.
//
#ifndef BB_TOKEN_H
#define BB_TOKEN_H
#include <set>
#include <regex>
#include <codecvt> //utf-8
#include "bb/Log.h"

namespace bb{
    class Token{
        unsigned max_size_ = 10;
        std::string token_db_path_ = "./WM_token.db";
        std::set<std::array<uint64_t,2>> token_{};
        Token();
        ~Token();
    public:
        static Token &obj();
        void push(std::array<uint64_t,2> &token);
        bool is(std::array<uint64_t,2> &token);
        bool rm(std::array<uint64_t,2> &token);
        static std::string UnicodeToUTF8(const std::wstring &wstr){
            std::string ret;
            try {
                std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
                ret = wcv.to_bytes(wstr);
            } catch (const std::exception & e) {
                //std::cerr << e.what() << std::endl;
            }
            return ret;
        }
        static std::wstring UTF8ToUnicode(const std::string &str){
            std::wstring ret;
            try {
                std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
                ret = wcv.from_bytes(str);
            } catch (const std::exception & e) {
                //std::cerr << e.what() << std::endl;
            }
            return ret;
        }
    };
}
#endif //BB_TOKEN_H