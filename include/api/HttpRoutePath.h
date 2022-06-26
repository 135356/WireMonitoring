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
#include "mysql_orm/mode/wireM_user.hpp"
#include "api/Token.h"

class HttpRoutePath {
    HttpRoutePath() = default;

    ~HttpRoutePath() = default;

    //注册
    static bool logOn(const char *accounts, const char *password);

    //登陆
    static bool logIn(const char *accounts, const char *password, std::array<uint64_t, 2> &token);

    //token登陆
    static bool logInToken(const char *token, const char *token_1);

public:
    static HttpRoutePath &obj() {
        static HttpRoutePath http_route_path;
        return http_route_path;
    }
    //处理请求给body赋值
    /*
     * r_data 接收到的数据
     * s_data 要发送给客户的数据
     * r_size 要发送的数据大小
     * */
    std::map<std::string, void (*)(std::map<std::string, std::string> &, std::string &, size_t &)> route = {
            {"/test/get",   [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
                //s_data = R"({"state":-1})"; //失败状态
                //s_data = R"({"state":0})"; //成功状态
                bb::Time::sleep(10);

                s_data.resize(1024);
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "get请求成功");
                s_data.resize(s_size);
            }},
            {"/test/post",  [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
                std::string r_str = "{";
                for (auto &v: r_data) {
                    r_str += '"' + v.first + "\":\"" + v.second + "\",";
                }
                s_data = r_str + "\"state\":0,\"msg\":\"post请求成功\"}"; //成功状态
                s_data.resize(1024);
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "post请求成功");
                s_data.resize(s_size);
            }},
            {"/logOn",      [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
                s_data.resize(1024);
                if (logOn(r_data["accounts"].c_str(), r_data["password"].c_str())) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "注册成功");
                } else {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "注册失败");
                }
                s_data.resize(s_size);
            }},
            {"/logIn",      [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
                s_data.resize(1024);
                std::array<uint64_t, 2> token{};
                if (logIn(r_data["accounts"].c_str(), r_data["password"].c_str(), token)) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s","token":"%lu","token_1":"%lu"})", 0, "登陆成功", token[0], token[1]);
                } else {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "登陆失败");
                }
                s_data.resize(s_size);
            }},
            {"/logInToken", [](std::map<std::string, std::string> &r_data, std::string &s_data, size_t &s_size) {
                s_data.resize(1024);
                if (logInToken(r_data["token"].c_str(), r_data["token_1"].c_str())) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "登陆成功");
                } else {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "登陆失败");
                }
                s_data.resize(s_size);
            }},
    };
};

#endif //BB_HTTPROUTEPATH_HPP