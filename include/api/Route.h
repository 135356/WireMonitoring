// http协议的路由地址
// Created by 邦邦 on 2022/6/17.
#ifndef WIREMONITORING_ROUTE_H
#define WIREMONITORING_ROUTE_H
#include <map>
#include <string>
#include <iostream>
#include <regex>
#include "bb/Time.h"
#include "bb/Token.h"
#include "bb/FloodIP.hpp"
#include "mode/wireM_user.hpp"
#include "mode/wireM_uploadFile.hpp"

class Route{
    Route() = default;
    ~Route() = default;
    //注册
    static int logOn(const char *accounts, const char *password);
    //登陆
    static int logIn(const char *accounts, const char *password, std::string &token);
    //修改密码
    static int changePassword(const char *accounts, const char *password, const char *new_password,std::string &token);
public:
    static Route &obj(){
        static Route bb_http_route_path;
        return bb_http_route_path;
    }
    //access_token验证
    static bool accessTokenVerification(unsigned &client_ip,std::string &access_token);
    //文件路由请求(客户端上传文件)
    std::map<std::string, void (*)(std::map<std::string,std::string> &,std::string &,size_t &)> route_upload = {
        {"/png_file", [](std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            int state = wireM_uploadFile::obj().insert({{"accounts",r_data["accounts"]},{"file_size",r_data["file_size"]},{"file_type",r_data["file_type"]},{"file_name",r_data["file_name"]},{"file_saved_name",r_data["file_saved_name"]}});
            s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s","file_path":"%s"})", 0, "上传成功",r_data["file_saved_name"].c_str());
            s_data.resize(s_size);
        }}
    };
    /*
     * r_data 接收到的数据
     * s_data 要发送给客户端的数据
     * r_size 要发送的数据大小
     * */
    std::map<std::string, void (*)(unsigned &,std::map<std::string,std::string> &,std::string &,size_t &)> route = {
        {"/logon",      [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(!FloodIP::obj().b10.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
                s_data.resize(s_size);
                return ;
            }

            int state = logOn(r_data["accounts"].c_str(), r_data["password"].c_str());
            if (state == 0) {
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "注册成功");
            } else if(state == -4){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "帐号已存在");
            }else{
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "注册失败");
            }
            s_data.resize(s_size);
        }},
        {"/login",      [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(!FloodIP::obj().a10.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
            }else{
                std::string token{};
                int state = logIn(r_data["accounts"].c_str(), r_data["password"].c_str(), token);
                if (state == 0) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s","token":"%s"})", 0, "登陆成功", token.c_str());
                } else {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "登陆失败");
                }
            }
            s_data.resize(s_size);
        }},
        {"/login_token", [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(accessTokenVerification(client_ip,r_data["token"])){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "登陆成功");
            }else{
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "登陆失败");
            }
            s_data.resize(s_size);
        }},
        {"/logout", [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if (bb::Token::obj().rm(r_data["token"])) {
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "登出成功");
            } else {
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "登出失败");
            }
            s_data.resize(s_size);
        }},
        {"/change_password", [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(!FloodIP::obj().a10.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
            }else{
                int state = changePassword(r_data["accounts"].c_str(), r_data["password"].c_str(),r_data["new_password"].c_str(),r_data["token"]);
                if (state == 0) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "修改成功");
                } else {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "修改失败");
                }
            }
            s_data.resize(s_size);
        }}
    };
};

#endif