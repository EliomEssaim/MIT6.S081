#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if( argc <= 1){
        char *emsg = "Please input an arugemt to determine sleep time.\n";
        // fprintf(2, "arugemt error\n");
        write(1,emsg, strlen(emsg));
        exit(1);
    }
    int clock_cnt = atoi(argv[1]);
    sleep(clock_cnt);
    exit(0);
}
