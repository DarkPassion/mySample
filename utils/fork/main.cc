#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void run_main();
void signal_handler(int s);
void init_signal();

// gcc -lstdc++ main.cc


void run_main()
{
    init_signal();
    
    pid_t pid;
    if((pid=fork())<0) {
        printf("fork == error!");
        return;
    }
    
    if (pid) {
        // parent process
        
        pid_t p = getpid();
        printf("pid = %d %d\n", p, pid);
        
        kill(pid, SIGHUP);
        
        while(1) {
            usleep(400* 1000);
        }
        
        
    } else {
        // child process
        pid_t p = getpid();
        printf("pid = %d", p);
        
        while (1) {
            usleep(400 * 1000);
        }
    }

}

void init_signal()
{
    signal(SIGHUP, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);


}

void signal_handler(int s)
{
    printf("signal_hander recv %d \n", s);
    
}


int main()
{
    run_main();

    return 0;
}


