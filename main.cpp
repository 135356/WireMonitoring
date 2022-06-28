#include "bb/tcp/Http.hpp"

int main(int argc, char *argv[]) {
    if(argc > 1){
        if(strncmp(argv[1],"stop",4) == 0){
            bb::Http::forkKill();
        }else if(strncmp(argv[1],"test",4) == 0){
            bb::Http::test();
        }else if(strncmp(argv[1],"start",3) == 0){
            bb::Http::run();
        }
    }

    return 0;
}