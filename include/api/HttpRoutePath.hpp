// http协议的路由地址
// Created by 邦邦 on 2022/6/17.
//
#ifndef BB_HTTPROUTEPATH_HPP
#define BB_HTTPROUTEPATH_HPP
#include <map>
#include <string>
#include <iostream>
#include <regex>
#include "bb/Time.h"
#include "bb/Base64.h"
#include "bb/tcp/AnalyseHttp.h"
#include "mysql_orm/mode/wireM_user.hpp"

class HttpRoutePath{
    HttpRoutePath()=default;
    ~HttpRoutePath()=default;
    static bool logIn(std::string &accounts,std::string &password){
        //帐号密码正则验证
        if(accounts.empty()){
            return false;
        }else{
            std::regex regex_a("^[a-zA-Z0-9|@]{6,16}$");
            if(!std::regex_search(accounts,regex_a)){
                return false;
            }
        }
        if(password.empty()){
            return false;
        }else{
            std::regex regex_a("^.{6,16}$");
            if(!std::regex_search(password,regex_a)){
                return false;
            }
        }
        //密码时间验证
        std::string password_ssl = bb::Base64::de(password.c_str());
        password = password_ssl.substr(0,password_ssl.size()-2);
        uint64_t password_time = bb::Time::getTime()/60/60;
        uint8_t password_key{},secret_key = password_time - (password_time/100)*100;
        try{
            password_key = std::stoi(password_ssl.substr(password_ssl.size()-2));
        }catch(...){
            perror("密码类型错误");
        }
        if(password_key != secret_key){
            return false;
        }
        //mysql数据库验证
        wireM_user::obj().where("email",accounts)->show();
        //成功http返回token


        return true;
    }
public:
    static HttpRoutePath &obj(){
        static HttpRoutePath http_route_path;
        return http_route_path;
    }
    //处理请求给body赋值
    /*
     * r_data 接收到的数据
     * s_data 要发送给客户的数据
     * r_size 要发送的数据大小
     * */
    std::map<std::string,void (*)(std::map<std::string,std::string> &,std::string &,size_t &)> route={
            {"/test/get",[](std::map<std::string,std::string> &r_data,std::string &s_data,size_t &r_size){
                //s_data = R"({"state":-1})"; //失败状态
                //s_data = R"({"state":0})"; //成功状态
                bb::Time::sleep(10);

                s_data.resize(1024);
                r_size = sprintf(&s_data[0],R"({"state":%d,"msg":"%s"})",0,"get请求成功");
                s_data.resize(r_size);
            }},
            {"/test/post",[](std::map<std::string,std::string> &r_data,std::string &s_data,size_t &r_size){
                std::string r_str="{";
                for(auto &v:r_data){
                    r_str += '"'+v.first+"\":\""+v.second+"\",";
                }
                s_data = r_str+"\"state\":0,\"msg\":\"post请求成功\"}"; //成功状态
                s_data.resize(1024);
                r_size = sprintf(&s_data[0],R"({"state":%d,"msg":"%s"})",0,"post请求成功");
                s_data.resize(r_size);
            }},
            {"/logIn",[](std::map<std::string,std::string> &r_data,std::string &s_data,size_t &r_size){
                std::cout<<"aaa"<<std::endl;
                std::string r_str="{";
                for(auto &v:r_data){
                    r_str += '"'+v.first+"\":\""+v.second+"\",";
                }

                s_data = r_str+"\"state\":0,\"msg\":\"post请求成功\"}"; //成功状态
                s_data.resize(1024);
                r_size = sprintf(&s_data[0],R"({"state":%d,"msg":"%s"})",0,"post请求成功");
                s_data.resize(r_size);
            }},
    };
};

#endif //BB_HTTPROUTEPATH_HPP