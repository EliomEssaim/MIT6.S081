#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

void filterPrime(int my_prime, int leftN){
    fprintf(0,"prime %d\n", my_prime);
    // close(0);
    // close(1);
    int num;
    char first_prime = 1;
    int subpipe[2];
    while(read(leftN, &num, INT_LEN)){
        if(num % my_prime != 0){
            if(first_prime){
                first_prime = 0;
                pipe(subpipe);
                if(fork()==0){
                    close(subpipe[WR]);
                    filterPrime(num, subpipe[RD]);
                }
                else{
                    close(subpipe[RD]);
                }
                continue;
            }
            write(subpipe[WR], &num, INT_LEN);
        }
    }
    close(leftN);
    close(subpipe[WR]);
    wait(0);
    exit(0);
}

int
main(int argc, char *argv[])
{
    char first_prime = 1;
    int m_prime = 2;
    fprintf(0,"prime %d\n", m_prime);
    // close(0);
    // close(1);
    // 管道可以提前写，写完了再wait。我这么写是看成一个个传过去了。所以还要想一想wait(0)要放哪里。
    int subpipe[2];
    for(int num = 3; num <= 35; num++){
        if(num % m_prime != 0){
            if(first_prime){
                first_prime = 0;
                pipe(subpipe);
                if(fork()==0){
                    close(subpipe[WR]);
                    filterPrime(num, subpipe[RD]);
                }
                else{
                    close(subpipe[RD]);
                }
                continue;
            }
            write(subpipe[WR], &num, INT_LEN);
        }
    }
    close(subpipe[WR]);
    wait(0);
    exit(0);
}
