#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void run_main();
void signal_handler(int s);
void init_signal();


void run_main2();

void run_main3();

void run_main4();

// gcc -lstdc++ main.cc


void run_main()
{
    init_signal();
    
    pid_t pid = 0;
    if((pid=fork())<0) {
        printf("fork == error!");
        return;
    }
    
    if (pid > 0) {
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


void run_main2()
{

    pid_t pid = 0;
    
    int count = 10;
    while (count > 0) {
        
        if ((pid = fork()) < 0) {
            printf("run main2 fork error ! \n");
            break;
        }
        
        if (pid > 0) {
            // parent process
            int status;
            waitpid(pid, &status, 0);
            printf("child process exit!! [%d %d] \n", getpid(), pid);
            count--;
        } else {
            // child process
            
            for (int i = 0; i < 10; i++) {
                printf("child process [%d] %d \n", getpid(), i);
                usleep(200 * 1000);
            }
            
            exit(0);
        }
    }
}



void run_main3()
{
    pid_t pid = 0;
    while (1) {
        
        char c[64] = {0};
        fgets(c, sizeof(c), stdin);
        printf("c = %s", c);
        
        if (c[0] == 'c') {
        
            if ((pid = fork()) < 0) {
                printf("error fork ! \n");
                break;
            }
            
            if (pid > 0) {
                // parent process
                printf("parent prcess %d %d \n", getpid(), pid);
            } else {
            
                printf("child process %d \n", getpid());
                while (1) {
                    usleep(400 * 1000);
                }
            }
            
        } else if (c[0] == 'k') {
        
            int status;
            kill(pid, SIGABRT);
            waitpid(pid, &status, 0);
            printf("child process exit ! %d %d\n", pid, status);
            
        }
        
        
    }

}


void run_main4()
{

    pid_t pid = 0;
    int fd[2] = {0};
    
    
    pipe(fd);
    
    if ((pid = fork()) < 0) {
        printf("run_main4 fork error ! \n");
        return;
    }
    
    if (pid > 0) {
        // parent process
        printf("parent process %d %d \n", getpid(), pid);
        close(fd[0]);
        
        while (1) {
        
            char c[64] = {0};
            fgets(c, sizeof(c), stdin);
            
            int nwrite = write(fd[1], c, strlen(c));
            if (nwrite > 0) {
                printf("parent process %d %.*s\n", getpid(), nwrite, c);
            } else {
                printf("parent process %d write error %d! \n", getpid(), errno);
                
                if (errno == EAGAIN) {
                    continue;
                }
                break;
            }
            
        }
        
    } else {
        // child process
        printf("child process %d \n", getpid());
        close(fd[1]);
        
        while (1) {
            char c[64] = {0};
            int nread = read(fd[0], c, sizeof(c));
            if (nread > 0) {
                printf("child process %d %.*s\n", getpid(), nread, c);
            } else {
                // read error
                if (errno == EAGAIN) {
                    continue;
                }
                
                printf("child process %d read error %d\n", getpid(), errno);
                break;
                
            }
            
            
        }
        
    }
  
}


int main()
{
    //run_main();

    //run_main2();
    
    //run_main3();
    
    run_main4();
    
    return 0;
}


