#include <iostream>
#include "Work.h"

int main(int argc, char *argv[]) {
    if(argc > 1){
        if(strncmp(argv[1],"stop",4) == 0){
            bb::Work::forkKill();
        }else if(strncmp(argv[1],"test",4) == 0){
            bb::Work::test();
        }else if(strncmp(argv[1],"start",3) == 0){
            bb::Work::run();
        }
    }else{
        bb::Work::test();
    }
    return 0;
}