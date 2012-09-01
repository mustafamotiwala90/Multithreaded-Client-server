

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>
#include<time.h>
#include<semaphore.h>
#include<alloca.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<signal.h>
#include<sys/file.h>
#include<sys/wait.h>
#include<sys/utsname.h>
struct request
{
    int socket;
    int filesize;
    char *command;
    char *filename;
    FILE *stream;
    struct request * next;
};
struct request req_array[100];
pthread_t *threads;
char *scheduling_type;
int multithreads;
int threadnumber=4,timethread=60,port_no=8080,flag,flagscheduling;
char *dir;
char *dir1;
//char filename[100[200];
int filesize[100];
//struct request socket_array[100];
int my_socket;
int i=0,start=0;
char *str;
sem_t semaphore_name,semaph1,semaph2;
FILE *operatinglogger;
char loggerfilename[20]="logging.txt";
//char *loggerfilename
char ipaddress[25];
char *reqparsing;
char * thread_time;
char * thread_time2;
int logging_flag = 0;
void do_cat(struct request *);
void logging_function();
