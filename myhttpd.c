#include "myhttpd.h"

void* server_thread_function(void* cst)
{
	int bufsize = 1024;
    int cont;
    char buffer[4096];
    char *fname;
    int fd;
    char *request_type_options;
    sem_wait(&semaph1);
    while(start==i);
    multithreads=multithreads-1;
    do_cat(&req_array[start]);
    request_type_options=req_array[start].command;
    fname=req_array[start].filename;

/*------------comparing the request options------------------*/

    if(strcmp(request_type_options,"GET")==0)
   {
     puts(fname);
     if((fd=open(fname,O_RDONLY))<0)
     {
      perror("ERROR NO 404 : File Open Failed");
     }
     while((cont=read(fd, buffer, bufsize))>0)
     {
      write(req_array[start].socket,buffer,cont);
     }
     close(req_array[start].socket);
   }

    start=start+1;
    sem_post(&semaph1);
    return(0);
}


void request_data_insertion(char* request_type_options,char* fname,int fsize,int new_socket)
{
  req_array[i].command=strdup(request_type_options);
  req_array[i].filename= strdup(fname);
  req_array[i].filesize=fsize;
  req_array[i].socket=new_socket;
  req_array[i].next=NULL;
   i=(i+1)%100;
   puts(req_array[i-1].filename);
   printf("%d",i);
}


void print_data(struct request t1)
{
  printf("\n\nThe contents of the structure are :\n");
  printf("\nRequest Type : %s",t1.command);
  printf("\nRequest Filename : %s",t1.filename);
  printf("\nFilesize : %d\n",t1.filesize);
}


void do_cat(struct request *t1)
{
	int buffer_size=1000000;
	char *buffer1=malloc(buffer_size);
	char http_response_string[200];
	char* fname=t1->filename;
	char* s;
	int new_sock=t1->socket;
	struct stat st;
	time_t lmt;
	time_t timestamp=time(NULL);
	char* server="Apache";
	int length;
	char b[20];
	char image[]="image/gif";
    char texthtml[]="text/html";
    stat(fname,&st);
    lmt=st.st_mtime;
    s=strrchr(fname,'.');
    s=s+1;
    length=t1->filesize;

    sprintf(b,"%d",length);

    strcpy(http_response_string,"HTTP/1.1 200 OK\n");
	strcpy(http_response_string,"Today's Date: ");
	strcpy(http_response_string,ctime(&timestamp));
	strcpy(http_response_string,"\n");
	strcpy(http_response_string,"Server: \n");
	strcpy(http_response_string,server);
	strcpy(http_response_string,"\n");
	strcpy(http_response_string,"Last-Modified-Time is : ");
	strcpy(http_response_string,ctime(&lmt));
	strcpy(http_response_string,"\n");
	strcpy(http_response_string,"Content-Type is : ");

	if(strcmp(s,"jpg")==0)
      {
		strcpy(http_response_string,image);
      }
     else
       {
      strcpy(http_response_string,texthtml);
       }

	strcpy(http_response_string,"\n");
	strcpy(http_response_string,"Content-Length: ");
	strcpy(http_response_string,b);
	strcpy(http_response_string,"\n");

	printf("\nContents:\n");
	printf("\nDate : %s",ctime(&timestamp));
	printf("\nServer : %s",server);
	printf("\nLast Modified : %s",ctime(&lmt));
	printf("\nContent-Type : %s",s);
	printf("\nContent-Length : %d",length);



	if(strcmp(t1->command,"HEAD")==0)
 {
                  printf("HEAD request was given by the user \n");
                  printf("\n Metadata : \n");
                  printf("\n DATE : %s",ctime(&timestamp));
                  printf("\n SERVER : %s ",server);
                  printf("\n LAST MODIFIED : %s",ctime(&lmt));
                  printf("\n CONTENT TYPE : %s",s);
                  printf("\n CONTENT LENGTH : %d",length);
 }


	if(strcmp(s,"jpg")==0)

	{
			FILE *fp=fopen(fname,"rb");
			fread(buffer1,1,buffer_size,fp);
			write(new_sock,buffer1,buffer_size);
	}

		write(new_sock,http_response_string,strlen(http_response_string));

}

/*--------funtion used to sort the request array in SJF sorting ---------*/

void SJF_sort()
{
 int p=0,q=0;
 struct request temp;
 for(p=start;p<i;p++)
  {
   for(q=start;q<i-(p+1);q++)
    {
      if(req_array[q].filesize>req_array[q+1].filesize)
       {
    	  temp.socket=req_array[q].socket;
    	  req_array[q].socket=req_array[q+1].socket;
    	  req_array[q+1].socket=temp.socket;
    	  temp.filesize=req_array[q].filesize;
    	  req_array[q].filesize=req_array[q+1].filesize;
    	  req_array[q+1].filesize=temp.filesize;
    	  temp.command=strdup(req_array[q].command);
    	  req_array[q].command=strdup(req_array[q+1].command);
    	  req_array[q+1].command=strdup(temp.command);
    	  temp.filename=strdup(req_array[q].filename);
    	  req_array[q].filename=strdup(req_array[q+1].filename);
    	  req_array[q+1].filename=strdup(temp.filename);

       }

    }

  }
}

/*----------function to put the attributes of threads in the particular thread------*/

void* threading_data(void* ask)
{
    sleep(timethread);
    printf("FINISHED WAITING");
          int ind=0;
          pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        for(ind=0;ind<threadnumber;ind++)
        {
        	pthread_create(&threads[ind],&attr,&server_thread_function,(void**)&ind);
        }
return(0);
}

/*--------------function used for the creating of sockets and inserting and parsing the data and printing the data response---------*/

void* server_data(void* cst)
{
	int new_socket;
	int addrlen;
	char fname[256];
	char *request_type_options,*filename,filename1[20],req[20];
    int i,fsize;
    struct stat st;
    char *tempdir;
    struct sockaddr_in client;

    while(1)
    {
    	listen(my_socket,3);
    	addrlen = sizeof(struct sockaddr_in);
    	new_socket = accept(my_socket,(struct sockaddr*)&client,&addrlen);
    	time_t timethread1 = time(NULL);
    	thread_time = ctime(&timethread1);

    	if (new_socket > 0)
    	printf("The Client %s is Connected...\n",inet_ntoa(client.sin_addr));
    	strcpy(ipaddress,inet_ntoa(client.sin_addr));
    	recv(new_socket,req, 255,0);
    	request_type_options = strtok(req," ");
    	reqparsing=strdup(req);
    	filename = strtok(NULL," ");
    	int j=0;

		  for(i=1;i<strlen(filename);i++)
		  {
			  filename1[j++]=filename[i];
		  }
	filename1[j]='\0';
   printf("\n %s \n %s \n",request_type_options,filename1);
   strcpy(fname,filename1);
   if(strncmp(fname,"~",1)==0)
   {
	   tempdir=strndup(fname+1,(strlen(fname)-2));
	   dir=strdup(tempdir);
	   flag = 1;
   }
   else
   {
  if(flag==1)
       {
      strcat(dir,"/");
      strcat(dir,fname);
      strcpy(fname,dir);
      dir = strdup(tempdir);
       }
   stat(fname,&st);
   fsize=st.st_size;
   printf("\n size = %d\n",fsize);
   sem_wait(&semaphore_name);
   request_data_insertion(request_type_options,fname,fsize,new_socket);
   multithreads=multithreads+1;
   if(flagscheduling==1)
     {
      SJF_sort();
     }
   if(logging_flag == 1)
    {
   logging_function();
    }
   sem_post(&semaphore_name);
}
}
}


/*------function to implement the thread pool which creates a pool of threads--------*/
void thread_pool()
{
	threads=(pthread_t *)malloc(sizeof(pthread_t)*threadnumber);
	multithreads=0;
	i=0;
	return;
}

/*-----logging option function assigned by the user with the -l flag-------*/

void logging_function()
{
	operatinglogger = fopen(loggerfilename,"a+");
	if(operatinglogger!= NULL)

	{
	fprintf(operatinglogger, "\n %s - [%s] - [%s] - %s /%s HTTP/1.0 %d \n\n", ipaddress, thread_time, thread_time2, reqparsing,req_array[start].filename, req_array->filesize);
	}

	fclose(operatinglogger);
}

/*-------the main function-------*/

int main(int argc,char* argv[])
{
  pthread_t queue;
  pthread_t schedule;
  int new_socket;
  char* ch;
  char option_to_exit[10];
  int z=0;
  struct sockaddr_in address;
  pthread_attr_t attr;
  for(z=0;z<argc;z++)
  {
  ch=strdup(argv[z]);

  switch(ch[1])
  {
  case 'p':
  printf("\n the value of the particular thread or the number is  %d\n",z);
  port_no=atoi(argv[z+1]);
  printf("connected to port %d",port_no);
  break;

  case 'h':
  printf("\n	1.) −d : Enter debugging mode. \n");
  printf("\n	2.) −h : Print a usage summary with all options and exit.\n");
  printf("\n	3.) −l file : Log all requests to the given file\n");
  printf("\n	4.) −p port : Listen on the given port \n");
  printf("\n	5.) −r directory : Set the root directory for the http server to dir \n");
  printf("\n	6.) −t time : Set the queuing time to time seconds \n");
  printf("\n	7.) −n threadnumber:creating the number of threads : \n");
  printf("\n	8.) −s scheduling: Set the scheduling policy. It can be either FCFS or SJF \n");

  printf("\n do you want to exit (yes/no)");
  scanf("%s",option_to_exit);

  if((strcmp(option_to_exit,"yes"))==0)
  {
  exit(0);
  }
  break;



	  case 'n':
		  threadnumber=atoi(argv[z+1]);
		  printf("\n number of threads created are  %d \n",threadnumber);
		  break;


	  case 't':
		  timethread=atoi(argv[z+1]);
		  printf("\n time for which idle %d \n",timethread);
		  break;


	  case 'l':
		  logging_flag = 1;
		  break;

	  case 'r':
		  dir=strdup(argv[z+1]);
		  printf("the directory option was called by the user");
		  flag=1;
		  break;


	  case 'd':
		  printf("\nEntering debug mode\n");
		  system("gdb ./a.out");
		  return 0;

	  case 's':
        scheduling_type=strdup(argv[z+1]);
        if(strcmp(scheduling_type,"SJF"))
        {

        	flagscheduling=1;
        }

	  default:printf("\n");
	  break;

  }
  }
  if((my_socket = socket(AF_INET,SOCK_STREAM,0)) > 0)
	  printf("The socket was created\n");
	  sem_init(&semaphore_name,0,1);
	  sem_init(&semaph1,0,1);
	  sem_init(&semaph2,0,1);
	  address.sin_family = AF_INET;
	  address.sin_addr.s_addr= INADDR_ANY;
	  address.sin_port = htons(port_no);

	  if(bind(my_socket,(struct sockaddr*)&address,sizeof(address)) == 0)
	  {
		  printf("Binded the Socket \n");
	  }
	  thread_pool();
	  pthread_attr_init(&attr);
	  pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);
	  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	  pthread_create(&schedule,&attr,&threading_data,(void**)&new_socket);
	  pthread_create(&queue,&attr,&server_data,(void**)&new_socket);
	  pthread_join(queue,NULL);
	  pthread_join(schedule,NULL);
//close(create_socket);
return(1);
}
