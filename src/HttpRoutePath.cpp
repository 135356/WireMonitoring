//
// Created by 邦邦 on 2022/6/24.
//
#include "api/HttpRoutePath.h"

bool HttpRoutePath::logOn(const char *accounts,const char *password){
    //帐号密码正则验证
    if(!accounts || !password){
        return false;
    }
    std::regex regex_a(R"(^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$)");
    if(!std::regex_search(accounts,regex_a)){
        return false;
    }
    if(strlen(password) != 32){
        return false;
    }
    int is = wireM_user::obj().insert({{"email",accounts},{"password",password}});
    if(is < 0){
        return false;
    }
    return true;
}
bool HttpRoutePath::logIn(const char *accounts,const char *password,std::array<uint64_t,2> &token){
    //帐号密码正则验证
    if(!accounts || !password){
        return false;
    }
    std::regex regex_a(R"(^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$)");
    if(!std::regex_search(accounts,regex_a)){
        return false;
    }
    if(strlen(password) != 32){
        return false;
    }
    //mysql数据库验证
    std::vector<std::map<std::string, std::string>> data = wireM_user::obj().where("email",accounts)->where("password",password)->get();
    if(data.empty()){
        return false;
    }

    unsigned char sMD5[16] = {0};
    MD5((const unsigned char*)password, strlen(password), sMD5);
    memmove(&token[0],sMD5,16);
    Token::obj().token.insert(token); //记录token

    //成功http返回token
    return true;
}
bool HttpRoutePath::logInToken(const char *key64,const char *key64_1){
    char *end = nullptr;
    std::array<uint64_t,2> key{strtoul(key64,&end,10),strtoul(key64_1,&end,10)};
    if(key[0] < 1 || key[1] < 1){return false;}

    return Token::obj().token.count(key);
}
/*bool HttpRoutePath::logIn(const char *accounts,const char *password){
    //帐号密码正则验证
    if(!accounts || !password){
        return false;
    }
    std::regex regex_a("^[a-zA-Z0-9]{6,16}$");
    if(!std::regex_search(accounts,regex_a)){
        return false;
    }
    regex_a = std::regex("^.{6,16}$");
    if(!std::regex_search(password,regex_a)){
        return false;
    }
    //密码时效验证
    std::string password_s = bb::Base64::de(++password);
    char password_time[3];memmove(password_time, &password_s[0], 2);
    char *end = nullptr;uint8_t password_time_i2=strtol(password_time, &end, 10);
    uint64_t time = bb::Time::getTime()/60/60;
    uint8_t time_i2 = time - (time/100)*100;
    if(password_time_i2 != time_i2){
        return false;
    }
    const char *password_src = &password_s[2];

    //mysql数据库验证
    wireM_user::obj().where("email",accounts)->show();
    //成功http返回token


    return true;
}*/