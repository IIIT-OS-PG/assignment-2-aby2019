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
#include<bits/stdc++.h>
#define buff_size 512
using namespace std;
map <string,pair<string,string> > m;

struct socket_info{
	string port;
	string addr;
};
void  *doit(void * value)
 {


 	int *sockfd=(int*) value;
	char buffer[buff_size];
	socket_info info;
	recv(*sockfd ,buffer,buff_size, 0);//file name
	string file_name(buffer);
	int ch;
	 
	 recv(*sockfd,&file_name,sizeof(file_name),0);//download or upload
	cout<<"at tracker: file_name\n "<<file_name<<endl;
	recv(*sockfd,&ch,sizeof(int),0);//download or upload
	cout<<"ch="<<ch<<endl;
	memset ( buffer ,'\0', buff_size);
	// cout<<"enter port_no";
	// cin>>buffer;
	
	if(ch==1)
	{	
		auto it=m.find(file_name);
		if(it == m.end())
		{
			cout<<"file is not present with any peer";
			strcpy(buffer,"0000");

				send(*sockfd,&buffer,sizeof(buffer),0);
				memset ( buffer ,'\0', buff_size);

		} 
		else
		{
			strcpy(buffer,m[file_name].first.c_str());
			cout<<"in map"<<buffer<<endl;
			int n=send(*sockfd,&buffer,sizeof(buffer),0);
			if(n==-1)
			cerr<<"error in sending"<<endl;
		}
	// int n=send(*sockfd,(void *)&info,sizeof(info), 0);
	// if(n==-1)
	// 	cerr<<"error in sending";
	//strcpy(buffer,peer_socket.port.c_str());    
    }
    else if(ch==2)
   {
    recv(*sockfd,buffer,sizeof(buffer),0);
    cout<<"at tracker: port no is "<<buffer<<endl;//port no of peer to add in map
    m[file_name]={buffer,"127.0.0.1"};
	}
	close(*sockfd);
	//close( server_fd);
	return NULL;
 }
int main(int argc,char * argv[])
{
int server_fd=socket(AF_INET,SOCK_STREAM,0);
 string port_no=argv[1];

sockaddr_in client_addr;
client_addr.sin_family=AF_INET;
client_addr.sin_port=htons(stoi(port_no));
client_addr.sin_addr.s_addr=inet_addr(argv[2]);
// client_addr.sin_addr.s_addr=INADDR_ANY;
// inet_pton(AF_INET,"0.0.0.0",&server_addr.sin_addr);
if(bind(server_fd,(sockaddr*)&client_addr,sizeof(client_addr))==-1)
{
    cerr<<"cant bind to ip/port";
    
}
if(listen(server_fd,3))
{
    cerr<<"cant  listen";
}


while(1)
{
	cout<<"connected"<<endl;	
	int c=0;
	int addrlen=sizeof(sockaddr);
	int sockfd=accept(server_fd,(sockaddr*)&client_addr,(socklen_t*)&addrlen);
	pthread_t tid;
	pthread_create(&tid,NULL,&doit,&sockfd);
	pthread_join(tid,NULL);

}

    return 0;
}

 
