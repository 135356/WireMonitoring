// 上传文件的mysql数据库mode
// Created by 邦邦 on 2022/7/5.
#ifndef BB_BBBASICS_UPLOADFILE_H
#define BB_BBBASICS_UPLOADFILE_H
#include "mysql_orm/sql/dql.h"

class bbBasics_uploadFile:public bb::dql{
    bbBasics_uploadFile(){
        if(run_() != 0){
            bb::Log::obj().error("mode创建的时候出现问题");
        }
        update_();
    }
public:
    static bbBasics_uploadFile &obj(){
        static bbBasics_uploadFile bb_basics_upload_file;
        return bb_basics_upload_file;
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
        delTable();
        return 0;
    }
    int create_(){
        return createTable(table_name_,[](auto *data){
            data->string_("accounts")->nullable_()->comment_("帐号");
            data->string_("file_size")->nullable_()->comment_("文件大小");
            data->string_("file_type")->nullable_()->comment_("文件类型");
            data->string_("file_name")->nullable_()->comment_("文件名称");
            data->string_("file_saved_name")->nullable_()->comment_("文件保存名称");
            data->dateAt_();
        });
    }
    void update_(){
        //delDB_("bb");
        //delTable_();
    }
    void delTable_(const std::string &db_name={},const std::string &table_name={}){
        if(!db_name.empty()){
            DB_name_ = db_name;
        }
        if(!table_name.empty()){
            table_name_ = table_name;
        }
        useDB(DB_name_);
        delTable();
    }
    void delDB_(const std::string &db_name={}){
        if(!db_name.empty()){
            DB_name_ = db_name;
        }
        delDB(DB_name_);
    }
};

#endif