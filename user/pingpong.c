#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1
int
main(int argc, char *argv[])
{
    int fc[2],cf[2];
    pipe(fc);
    pipe(cf);
    
    if (fork() == 0) {
        close(fc[WR]);
        close(cf[RD]);

        char buf[4];
        read(fc[RD], buf,sizeof buf);
        close(fc[RD]);

        fprintf(0,"%d: received %s\n", getpid(),buf);

        fprintf(cf[WR], "pong");
        close(cf[WR]);
        exit(0);
    } else {
        char buf_[4];
        close(fc[RD]);
        close(cf[WR]);

        fprintf(fc[WR], "ping");
        close(fc[WR]);

        read(cf[RD],buf_, sizeof buf_);
        close(cf[RD]);

        fprintf(0,"%d: received %s\n", getpid(),buf_);
        exit(0);
    }
}
