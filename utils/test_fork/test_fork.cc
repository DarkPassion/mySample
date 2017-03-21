#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdarg.h>
#include <string.h>


// g++ test_fork.cc -o test_fork.out -lpthread
// pthread_sigmask
// http://man7.org/linux/man-pages/man3/pthread_sigmask.3.html

#define LOG_D(...) default_log("DEBUG", __VA_ARGS__);
#define LOG_E(...) default_log("ERROR", __VA_ARGS__);

// global var
FILE * glog = NULL;
char now_str[64] = {0};


// functions
void init_log();
void deinit_log();
void update_time_stamp();
void default_log(const char* level, const char* fmt, ...);
void* thread_enc(void* arg);
pthread_t create_signal_thread(void* args);
void write_pid_file(const char* file);


void init_log()
{
    if (glog == NULL) {

        glog = fopen("test_fork.log", "a");
    }

    update_time_stamp();
    fprintf(glog, "%s %s", now_str, "init log ================== \n");
}

void deinit_log()
{
    if (glog) {

        update_time_stamp();
        fprintf(glog, "%s %s", now_str, "deinit log ================== \n");
        fclose(glog);
    }
    glog = NULL;
}

void update_time_stamp()
{
        struct tm tm;
        time_t t = time(NULL);
        localtime_r(&t, &tm);
        memset(now_str, 0, sizeof(now_str));

        sprintf(now_str, "%04d/%02d/%02d %02d:%02d:%02d",
            1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

}

void default_log(const char* level, const char* fmt, ...)
{
    if (glog) {
        update_time_stamp();
        va_list args;
        va_start(args, fmt);
        fprintf(glog, "%s %s ", "DEBUG: ", now_str);
        vfprintf(glog, fmt, args);
        va_end(args);
        fflush(glog);
    }
}

void* thread_enc(void* arg)
{

    sigset_t* mask = (sigset_t*) arg;
    LOG_D("thread_enc begin == sigwait \n");
    while(true) {

        int err, signo;
        err = sigwait(mask, &signo);
        if (err != 0) {
            LOG_D("sigwait error !\n");
        }

        LOG_D("sigwait signo [%d] \n", signo);

        if (signo == SIGHUP) {
            LOG_D("sigwait got SIGHUP signal == \n")
        } else {
            LOG_D("sigwait got not SIGHUP signal == \n");
        }
    }
    return NULL;
}

pthread_t create_signal_thread(void* args)
{
    pthread_t tid;

    sigset_t* mask = (sigset_t*)args;

    sigemptyset(mask);
    sigaddset(mask, SIGINT);
    sigaddset(mask, SIGTERM);
    sigaddset(mask, SIGPIPE);
    sigaddset(mask, SIGHUP);
    sigaddset(mask, SIGUSR1);

    if (pthread_sigmask(SIG_BLOCK, mask, NULL) != 0) {
        LOG_D("thread_enc pthread_sigmask error \n");
    }


    if (pthread_create(&tid, NULL, thread_enc, mask) != 0) {
        printf("pthread_create fail! \n");
    }


    return tid;
}

void write_pid_file(const char* file)
{
    pid_t pid = getpid();

    char * fname = NULL;
    if (file == NULL) {
        fname = (char*) malloc(64);
        sprintf(fname, ".pid");
    } else {
        fname = strdup(file);
    }

    LOG_D("write pid file fname [%s %lld]\n", fname, (long long)pid);
    int fd = open(fname, O_WRONLY|O_CREAT, 0644);
    char buf[32] = {0};
    sprintf(buf, "%lld\n", (long long)pid);
    if (fd > 0 && (int)write(fd, buf, strlen(buf)) != (int)strlen(buf)) {
        LOG_E("write pid file error !");
        exit(1);
    }

    free(fname);
}


int main()
{

    pid_t pid = fork();
    if (pid < 0) {

        printf("system fork error ! \n");
    }

    if (pid > 0) {
        printf("this is parent process [%d %d]! \n", getpid(), pid);

        exit(0);
    } else {

        printf("this is child process [%d %d]! \n", getpid(), pid);
    }

    printf("child process run main loop ! \n");

    setsid();
    int fd;
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {

        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO) {
            close(fd);
        }
    } else {
        printf("open /dev/null fd fail ! \n");

    }

    init_log();
    write_pid_file(NULL);

    LOG_D("child process run ! \n");
    sigset_t mask;
    pthread_t tid;
    tid = create_signal_thread(&mask);

    void* ret;
    pthread_join(tid, &ret);

    deinit_log();

	return 0;
}




