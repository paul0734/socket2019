#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000
#define com "/w"
struct list_c {
	int c_socket;
	char name[CHATDATA];
};
void *do_chat(void *); //채팅 메세지를 보내는 함수
int pushClient(int, char *); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int); //클라이언트가 종료했을 때 클라이언트 정보 삭제
pthread_t thread;
pthread_mutex_t mutex;
struct  list_c list_c[MAX_CLIENT];
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";

int main(int argc, char *argv[ ])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n;
    int    res;
	 char name[CHATDATA];
    if(pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Can not create mutex\n");
        return -1;
    }
    s_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
        printf("Can not Bind\n");
        return -1;
    }
    if(listen(s_socket, MAX_CLIENT) == -1) {
        printf("listen Fail\n");
        return -1;
    }
    for(i = 0; i < MAX_CLIENT; i++)
        list_c[i].c_socket = INVALID_SOCK;
    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
		  if((n=read(c_socket, name, sizeof(name)))< 0){
				printf("nickname error\n");
				return -1;
		    }
        res = pushClient(c_socket, name);
        if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            pthread_create(&thread, NULL, do_chat, (void *)&c_socket);
        }
    }
}
void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
    int n, j;
	 
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
				char *t = NULL;
				char *name = NULL;
				char *message = NULL;
				if(strncasecmp(chatData,com, 2) == 0){
					t = strtok(chatData, " ");
					name = strtok(NULL, " ");
					message = strtok(NULL, "\0");
					printf("%s |%s| %s\n",t,name,message);
				}
			for(j=0;j<MAX_CLIENT;j++){            
			 if(list_c[j].c_socket != INVALID_SOCK){
						if(name != NULL){
							printf("ss\n");
							printf("list_c = %s name = %s\n",list_c[j].name, name);
							if(strcasecmp(list_c[j].name, name)==0){
								printf("plw\n");
								write(list_c[j].c_socket, message, strlen(message));			
							}
						}else{
							printf("cc\n");
							write(list_c[j].c_socket, chatData, n);
						}
			
			 }//if			
			}  //for          		
		
            		///////////////////////////////
            if(strstr(chatData, escape) != NULL) {
                popClient(c_socket);
                break;
           	}//if
        }//if
    }//while
}//ham
int pushClient(int c_socket, char *name) {
	int j;
	for(j=0; j<MAX_CLIENT;j++){
		if(list_c[j].c_socket == INVALID_SOCK){
			pthread_mutex_lock(&mutex);
			list_c[j].c_socket = c_socket; //ADD c_socket to list_c array.
			memset(list_c[j].name,0, CHATDATA);
			strcpy(list_c[j].name, name);		
			pthread_mutex_unlock(&mutex);			
			return j;
		}  	
	}if(j==MAX_CLIENT){
		return -1;
	}
    //return -1, if list_c is full.
    //return the index of list_c which c_socket is added.
}
int popClient(int c_socket)
{	
	int j;
	for(j=0;j<MAX_CLIENT;j++){
		if(list_c[j].c_socket != INVALID_SOCK){
			pthread_mutex_lock(&mutex);				
			list_c[j].c_socket=INVALID_SOCK;	
			pthread_mutex_unlock(&mutex);
			break;	
		}
	}
    close(c_socket);
	return 0;
}
