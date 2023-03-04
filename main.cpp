#include <iostream>
#include "Work.hpp"

int main(int argc, char *argv[]) {
    if(argc > 1){
        if(strncmp(argv[1],"stop",4) == 0){
            Work::obj().stopF(argv[0]);
        }else if(strncmp(argv[1],"test",4) == 0){
            Work::obj().testF();
        }else if(strncmp(argv[1],"start",3) == 0){
            Work::obj().formalF();
        }
    }else{
        Work::obj().testF();
    }
    return 0;
}