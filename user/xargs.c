#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define RD 0
#define WR 1
#define NULL 0
// #define print(format, ...) printf(format, ##__VA_ARGS__)
#define print(format, ...)

void deSpace(int* idx, int* pre_idx, char* buf){
    while(buf[*idx]==' ') (*idx)++;
    *pre_idx = *idx;
    return;
}


void cleanArgv(char *argv[],int oldstart, int new_start){
    while(new_start!=oldstart){
        argv[new_start] = NULL;
        new_start--;
    }
}

void ForkAndExecute(char *cmd, char *argv[]){
    print("start fork!\n");
    if(fork() == 0){
        argv[0] = cmd;
        print("child exec %s\n", cmd);
        print("child argv[1]: %s\n",argv[1]);
        print("child argv[2]: %s\n",argv[2]);
        print("child argv[3]: %s\n",argv[3]);
        exec(cmd, argv);
    }
    wait(0);
}

int
main(int argc, char *argv[])
{
    print("start sd!\n");
    char buf[512];
    read(0, buf, sizeof buf);
    print("read %s over\n", buf);
    int idx=0, pre_idx = 0;
    
    deSpace(&idx, &pre_idx, buf);
    char* new_argv[MAXARG];
    char* CMD = argv[1];

    int new_start = 2;
    for(; new_start < argc; new_start++)
        new_argv[new_start - 1] = argv[new_start];

    const int NEW_START = new_start - 1;
    new_start--;
    print("start parse! news_start is %d NEW_START is %d\n", new_start, NEW_START);
    while(buf[idx]){
        if(buf[idx]==' ' || buf[idx]=='\n'){
            int flag_last = buf[idx]=='\n'?1:0;
            buf[idx] = 0;
            new_argv[new_start] = &buf[pre_idx];
            print("update new_argv at pos %d, with val: %s \n", new_start,&buf[pre_idx]);
            new_start++; 
            pre_idx = idx + 1;
            idx++;
            deSpace(&idx, &pre_idx, buf);
            if(flag_last){
                ForkAndExecute(CMD, new_argv);
                cleanArgv(new_argv, NEW_START, new_start);
                new_start = NEW_START;
            }
        }
        else
            idx++;
    }
    exit(0);
}
