//
// Created by 邦邦 on 2022/4/26.
//
#ifndef MYSQLORM_DBA1_TEST_HPP
#define MYSQLORM_DBA1_TEST_HPP
#include "mysql_orm/sql/dql.h"

class wireM_user:public bb::dql{
    wireM_user(){
        if(run_() != 0){
            debug.error("mode创建的时候出现问题");
        }
        update_();
    }
public:
    static wireM_user &obj(){
        static wireM_user wireM_user;
        return wireM_user;
    }
protected:
    int run_(){
        std::array<std::string,2> obj_info = getName_();
        DB_name_ = obj_info[0];
        table_name_ = obj_info[1];
        if(createDB(DB_name_) == 0 && useDB(DB_name_) == 0){
            if(isTable(table_name_) == 0){
                if(create_() != 0){return -1;}
            }
            return useTable(table_name_);
        }
        return 0;
    }
    int create_(){
        return createTable(table_name_,[](auto *data){
            data->int_("phone")->nullable_()->comment_("手机号");
            data->int_("email")->nullable_()->comment_("邮箱");
            data->int_("grade")->nullable_()->comment_("等级");
            data->int_("password")->nullable_()->comment_("密码");
            data->dateAt_();
        });
    }
    void update_(){}
};

#endif //MYSQLORM_DBA1_TEST_HPP
