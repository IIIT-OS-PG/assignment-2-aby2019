#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<string>
#include <netinet/in.h>
#include<pthread.h>
#define buff_size 512

using namespace std;
void * server_doit(void * value)
 {
 	int *sockfd=(int*) value;
	 	FILE *fp=fopen("abd.mp4","wb");
	char buffer[buff_size];
	int file_size;
	recv(*sockfd,&file_size,sizeof(file_size),0);
	int n;
	while ((n = recv( *sockfd ,buffer,buff_size, 0) ) > 0  &&  file_size > 0)
	{
	
		fwrite (buffer , sizeof (char), n, fp);
		memset ( buffer ,'\0', buff_size);
		file_size = file_size - n;
	} 
	close(*sockfd);
	//close( server_fd);
	fclose (fp);
  return NULL;
 }

void *server(void * value)
 {

 	// char **argv= (char**) value;
 	int server_fd=socket(AF_INET,SOCK_STREAM,0);
 //string port_no=argv[1];
 	cout<<"server port"<<endl;
 	string port;
 	cin>>port;
// cout<<argv[1]<<endl;
// cout<<argv[2]<<endl;
sockaddr_in client_addr;
client_addr.sin_family=AF_INET;
client_addr.sin_port=htons(stoi(port));

client_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
// client_addr.sin_addr.s_addr=INADDR_ANY;
// inet_pton(AF_INET,"0.0.0.0",&server_addr.sin_addr);
if(bind(server_fd,(sockaddr*)&client_addr,sizeof(client_addr))==-1)
{
    cerr<<"cant bind to ip/port";
}
if(listen(server_fd,3))
{
    cerr<<"cant listen";
}


	while(1)
	{
		int c=0;
		int addrlen=sizeof(sockaddr);
		int sockfd=accept(server_fd,(sockaddr*)&client_addr,(socklen_t*)&addrlen);
		pthread_t tid;

		pthread_create(&tid,NULL,&server_doit,&sockfd);

		if(c>=3)
		{
			c=0;
			while(c<3)
			{
				pthread_join(tid,NULL);

			}

		}
	
 	}
}
void * client_doit(void * value)
{
	
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
        cerr<<"cannot create socket";
        return NULL;
	}
    //cout<<"client port";
    string port_no;
    cin>>port_no;

	  sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(stoi(port_no));
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(connect(sockfd,(sockaddr *)&server_addr,sizeof(server_addr))==-1)
    {
        cout<<"cannot connect";
        return NULL;
    }
    FILE *fp=fopen("abc.mp4","rb");
    fseek(fp,0,SEEK_END);
    int size =ftell(fp);
    rewind(fp);
    send(sockfd,&size,sizeof(size),0);
    char buffer[buff_size];
    int n;
    while((n=fread(buffer,sizeof(char),buff_size,fp))>0&&size >0)
    {
        send(sockfd,buffer,n,0);
        memset(buffer,'\0',buff_size);
        size=size-n;
    }
    cout<<"file_sent"<<endl;
    fclose(fp);
    close(sockfd);	
}

void * client(void * value)
{
	sleep(10);
  int i = 0;
  pthread_t tid[51];
  while(i< 50)
  {
    if( pthread_create(&tid[i], NULL, &client_doit, NULL) != 0 )
           cout<<"cannot create thread"<<endl;
    i++;
  }
  
  i = 0;
  while(i< 50)
   {
     pthread_join(tid[i++],NULL);
  }
}

int main(int argc,char * argv[])
{
pthread_t t_server,t_client;
pthread_create(&t_server,NULL,&server,NULL);
//server(argv);	

pthread_create(&t_client,NULL,&client,NULL);
pthread_join(t_client,NULL);

    return 0;
}

 
