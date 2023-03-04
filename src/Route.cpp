//
// Created by 邦邦 on 2022/6/24.
#include "Route.h"

bool Route::accessTokenVerification(unsigned &client_ip,std::string &access_token){
    if(!FloodIP::obj().a10.is(client_ip)){
        return false;
    }
    if (bb::secure::Token::obj().is(access_token)) {
        return true;
    } else {
        FloodIP::obj().a10.push(client_ip);
        return false;
    }
}
int Route::logOn(const char *accounts,const char *password){
    //帐号密码正则验证
    if(!accounts || !password){
        return -1;
    }
    std::regex regex_a(R"(^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$)");
    if(!std::regex_search(accounts,regex_a)){
        return -2;
    }
    if(strlen(password) != 32){
        return -3;
    }
    //mysql数据库验证
    std::vector<std::map<std::string, std::string>> data = bbBasics_user::obj().where("email",accounts)->get();
    if(!data.empty()){
        return -4;
    }
    int is = bbBasics_user::obj().insert({{"email",accounts},{"password",password}});
    if(is < 0){
        return -41;
    }
    return 0;
}
int Route::logIn(const char *accounts,const char *password,std::string &token){
    //帐号密码正则验证
    if(!accounts || !password){
        return -1;
    }
    std::regex regex_a(R"(^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$)");
    if(!std::regex_search(accounts,regex_a)){
        return -2;
    }
    if(strlen(password) != 32){
        return -3;
    }
    //mysql数据库验证
    std::vector<std::map<std::string, std::string>> data = bbBasics_user::obj().where("email",accounts)->where("password",password)->get();
    if(data.empty()){
        return -4;
    }
    //成功返回token
    bb::secure::Token::obj().push(accounts,token);
    return 0;
}
int Route::changePassword(const char *accounts, const char *password, const char *new_password,std::string &token){
    //帐号密码正则验证
    std::regex regex_a(R"(^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$)");
    if(!std::regex_search(accounts,regex_a)){
        return -2;
    }
    if(strlen(password) != 32 || strlen(new_password) != 32){
        return -3;
    }
    if (!bb::secure::Token::obj().is(token)) {
        return -40;
    }
    bb::secure::Token::obj().rm(token);
    //mysql数据库验证
    std::vector<std::map<std::string, std::string>> data = bbBasics_user::obj().where("email",accounts)->where("password",password)->get();
    if(data.empty()){
        return -4;
    }
    int is = bbBasics_user::obj().where("email",accounts)->update({{"password",new_password}});
    if(is < 0){
        return -41;
    }
    return 0;
}