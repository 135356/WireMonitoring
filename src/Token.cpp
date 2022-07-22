//
// Created by 邦邦 on 2022/6/27.
//
#include "Token.h"

namespace bb{
    Token::Token(){
        FILE *fp = fopen(token_db_path_.c_str(), "rb");
        if(!fp){return ;}
        while(true){
            std::string token{};uint8_t str_size{};
            if(fread(&str_size,1,1,fp) == 1){
                token.resize(str_size);
            }else{
                break;
            }
            if(fread(&token[0],1,str_size,fp) == str_size){
                token_.insert(token);
            }else{
                break;
            }
        }
        fclose(fp);
    }
    Token::~Token(){
        if(token_.empty()){return ;}
        FILE *fp = fopen(token_db_path_.c_str(), "wb");
        if(token_.size() > max_size_){
            bb::Log::obj().warn("token 大于 "+std::to_string(max_size_)+"条,已自动清理了token");
            fclose(fp);
            return ;
        }
        for(const std::string &v:token_){
            uint8_t str_size = v.size();
            if(fwrite(&str_size,1,1,fp) != 1 || fwrite(&v[0],1,str_size,fp) != str_size){
                bb::Log::obj().warn(token_db_path_+" 文件写入失败");
                break;
            }
        }
        fclose(fp);
    };
    Token &Token::obj(){
        static Token bb_token;
        return bb_token;
    }
    void Token::push(const char *accounts,const char *password,std::string &token){
        unsigned char md5[32] = {};
        MD5((unsigned char*)accounts, strlen(accounts), &md5[0]);
        MD5((unsigned char*)password, strlen(password), &md5[16]);
        uint64_t token_arr[4];
        memmove(&token_arr[0],&md5[0],8);
        memmove(&token_arr[1],&md5[8],8);
        memmove(&token_arr[3],&md5[24],8);
        token_arr[2]=bb::Time::getTime();
        token = std::to_string(token_arr[0])+std::to_string(token_arr[1])+'_'+std::to_string(token_arr[2])+std::to_string(token_arr[3]);
        token_.insert(token);
    }
    bool Token::is(std::string &token) const{
        return token_.count(token);
    }
    bool Token::rm(std::string &token){
        return token_.erase(token);
    }
}