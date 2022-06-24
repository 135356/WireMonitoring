#include <iostream>
#include <sys/wait.h>
#include "bb/tcp/Http.hpp"
#include "encrypt.hpp"

int main(int argc, char *argv[]) {
    std::string dst;
    encrypt::md5("sadfsdafhdsfkalsjfdslfadshfldsa;fakhfkljsd;lalfjh",dst);
    std::cout<<dst<<std::endl;

    //bb::Http http;
    //printf("%ld",bb::Time::getTime()/60/60);
    /*wireM_user::obj().insert({{"email","13536@sina.com"},{"password","v2"}});*/
    /*while(true){
        pid_t pid = fork();
        if(pid == 0){
            bb::Http http;
        }
        waitpid(pid, 0, 0);
    }*/

    return 0;
}