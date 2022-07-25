#include <iostream>
#include "Work.h"

int main(int argc, char *argv[]) {
    if(argc > 1){
        if(strncmp(argv[1],"stop",4) == 0){
            bb::Work::obj().stopF(argv[0]);
        }else if(strncmp(argv[1],"test",4) == 0){
            bb::Work::obj().testF();
        }else if(strncmp(argv[1],"start",3) == 0){
            bb::Work::obj().formalF();
        }
    }else{
        bb::Work::obj().testF();
    }
    return 0;
}