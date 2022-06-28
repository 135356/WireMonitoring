//
// Created by 邦邦 on 2022/6/24.
//
#include "api/HttpRoutePath.h"

int HttpRoutePath::logOn(const char *accounts,const char *password){
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
    int is = wireM_user::obj().insert({{"email",accounts},{"password",password}});
    if(is < 0){
        return -4;
    }
    return 0;
}
int HttpRoutePath::logIn(const char *accounts,const char *password,std::array<uint64_t,2> &token_arr){
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
    std::vector<std::map<std::string, std::string>> data = wireM_user::obj().where("email",accounts)->where("password",password)->get();
    if(data.empty()){
        return -4;
    }
    //成功返回token
    unsigned char md5[16] = {0};
    MD5((const unsigned char*)password, strlen(password), md5);
    memmove(&token_arr[0],md5,16);
    bb::Token::obj().push(token_arr);

    return 0;
}
bool HttpRoutePath::logInToken(const char *token,const char *token_1){
    char *end = nullptr;
    std::array<uint64_t,2> token_arr{strtoul(token,&end,10),strtoul(token_1,&end,10)};
    if(token[0] < 1 || token[1] < 1){return false;}
    return bb::Token::obj().is(token_arr);
}
bool HttpRoutePath::logOut(const char *token, const char *token_1){
    char *end = nullptr;
    std::array<uint64_t,2> token_arr{strtoul(token,&end,10),strtoul(token_1,&end,10)};
    if(token[0] < 1 || token[1] < 1){return false;}
    return bb::Token::obj().rm(token_arr);
}
int HttpRoutePath::changePassword(const char *accounts, const char *password, const char *new_password){
    //帐号密码正则验证
    if(!accounts || !password || !new_password){
        return -1;
    }
    std::regex regex_a(R"(^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$)");
    if(!std::regex_search(accounts,regex_a)){
        return -2;
    }
    if(strlen(password) != 32 || strlen(new_password) != 32){
        return -3;
    }
    //mysql数据库验证
    std::vector<std::map<std::string, std::string>> data = wireM_user::obj().where("email",accounts)->where("password",password)->get();
    if(data.empty()){
        return -4;
    }
    int is = wireM_user::obj().where("email",accounts)->update({{"password",new_password}});
    if(is < 0){
        return -41;
    }
    return 0;
}