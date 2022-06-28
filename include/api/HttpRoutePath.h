// http协议的路由地址
// Created by 邦邦 on 2022/6/17.
//
#ifndef BB_HTTPROUTEPATH_HPP
#define BB_HTTPROUTEPATH_HPP
#include <map>
#include <string>
#include <iostream>
#include <regex>
#include <openssl/md5.h>
#include "bb/Time.h"
#include "bb/Base64.h"
#include "bb/tcp/AnalyseHttp.h"
#include "bb/Token.h"
#include "mysql_orm/mode/wireM_user.hpp"
#include "WMFlood.hpp"

class HttpRoutePath {
    HttpRoutePath() = default;
    ~HttpRoutePath() = default;
    //注册
    static int logOn(const char *accounts, const char *password);
    //登陆
    static int logIn(const char *accounts, const char *password, std::array<uint64_t, 2> &token_arr);
    //token登陆
    static bool logInToken(const char *token, const char *token_1);
    //token登出
    static bool logOut(const char *token, const char *token_1);
    //修改密码
    static int changePassword(const char *accounts, const char *password, const char *new_password);

public:
    static HttpRoutePath &obj(){
        static HttpRoutePath bb_http_route_path;
        return bb_http_route_path;
    }
    /*
     * r_data 接收到的数据
     * s_data 要发送给客户端的数据
     * r_size 要发送的数据大小
     * */
    std::map<std::string, void (*)(std::map<std::string, std::string> &, std::string &, size_t &)> route = {
            {"/logOn",      [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
                s_data.resize(1024);
                uint32_t accounts_int{};memmove(&accounts_int,&r_data["accounts"][0],4);
                if(!WMFlood::obj().a10.pushIs(accounts_int)){
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
                    s_data.resize(s_size);
                    return ;
                }

                int state = logOn(r_data["accounts"].c_str(), r_data["password"].c_str());
                if (state == 0) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "注册成功");
                } else {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "注册失败");
                }
                s_data.resize(s_size);
            }},
            {"/logIn",      [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
                s_data.resize(1024);
                uint32_t accounts_int{};memmove(&accounts_int,&r_data["accounts"][0],4);
                if(!WMFlood::obj().a10.pushIs(accounts_int)){
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
                    s_data.resize(s_size);
                    return ;
                }

                std::array<uint64_t, 2> token_arr{};
                int state = logIn(r_data["accounts"].c_str(), r_data["password"].c_str(), token_arr);
                if (state == 0) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s","token":"%lu","token_1":"%lu"})", 0, "登陆成功", token_arr[0], token_arr[1]);
                } else {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "登陆失败");
                }
                s_data.resize(s_size);
            }},
            {"/logInToken", [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
                s_data.resize(1024);
                uint32_t accounts_int{};memmove(&accounts_int,&r_data["accounts"][0],4);
                if(!WMFlood::obj().b10.is(accounts_int)){
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
                    s_data.resize(s_size);
                    return ;
                }

                if (logInToken(r_data["token"].c_str(), r_data["token_1"].c_str())) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "登陆成功");
                } else {
                    WMFlood::obj().b10.push(accounts_int);
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "登陆失败");
                }
                s_data.resize(s_size);
            }},
            {"/logOut", [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
                s_data.resize(1024);
                if (logOut(r_data["token"].c_str(), r_data["token_1"].c_str())) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "登出成功");
                } else {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "登出失败");
                }
                s_data.resize(s_size);
            }},
            {"/changePassword", [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
                s_data.resize(1024);
                uint32_t accounts_int{};memmove(&accounts_int,&r_data["accounts"][0],4);
                if(!WMFlood::obj().a10.pushIs(accounts_int)){
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
                    s_data.resize(s_size);
                    return ;
                }

                int state = changePassword(r_data["accounts"].c_str(), r_data["password"].c_str(),r_data["new_password"].c_str());
                if (state == 0) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "修改成功");
                } else {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "修改失败");
                }
                s_data.resize(s_size);
            }},
    };
};

#endif //BB_HTTPROUTEPATH_HPP
/*{"/test/post",  [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
    //s_data = R"({"state":-1})"; //失败状态
    //s_data = R"({"state":0})"; //成功状态
    std::string r_str = "{";
    for (auto &v: r_data) {
        r_str += '"' + v.first + "\":\"" + v.second + "\",";
    }
    s_data = r_str + "\"state\":0,\"msg\":\"post请求成功\"}"; //成功状态
    s_data.resize(1024);
    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "post请求成功");
    s_data.resize(s_size);
}},*/