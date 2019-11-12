#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#define PORT 9000
#define BUFSIZE 100
char buffer[BUFSIZE] = "Hi, I am server";
char rcvBuffer[BUFSIZE];
char sendBuffer[BUFSIZE];
char sendBuffer2[BUFSIZE] = "안녕하세요, 만나서만가워요";
char sendBuffer3[BUFSIZE] = "내 이름은 이건우야";
char sendBuffer4[BUFSIZE] = "나는 23살이야";
char sendBuffer5[BUFSIZE] = "error";
char cmpBuffer[2][BUFSIZE];
char *t;
char *str[100];
char *token;
void do_service(int c_socket);
void sig_handler();
int c_socket, s_socket;
struct sockaddr_in s_addr, c_addr;
int len;
int n;
int i;
int lt;
int idx=0;
int cnt=0;
FILE *fp;
int pid;
char buff[255];
int numClient = 0;

int main(){
	
	signal(SIGCHLD, sig_handler);//첫번째 인자 : 시그널번호, 두번째 인자 : 첫번째 인자의 시그널이 발생했을 때 실행되는 함수
	
	// 1. 서버 소켓 생성
	//서버 소켓 = 클라이언트의 접속 요청을 처리(허용)해 주기 위한 소켓
	s_socket = socket(PF_INET, SOCK_STREAM, 0); //TCP/IP 통신을 위한 서버 소켓 생성
	
	//2. 서버 소켓 주소 설정
	memset(&s_addr, 0, sizeof(s_addr)); //s_addr의 값을 모두 0으로  초기화
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP 주소 설정
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	//3. 서버 소켓바인딩
	if(bind(s_socket,(struct sockaddr *) &s_addr, sizeof(s_addr)) == -1){ 
		//바인딩 작업 실패 시, Cannot Bind 메시지 출력 후 프로그램 종료
		printf("Cannot Bind\n");
		return -1;
	}
	
	//4.listen() 함수 실행
	if(listen(s_socket, 5) == -1){
		printf("listen Fail\n");
		return -1;
	}
	while(1){ //무한 루프
		len = sizeof(c_addr);
		printf("클라이언트 접속을 기다리는 중....\n");
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len); 
		//클라이언트의 요청이 오면 허용(accept)해 주고, 해당 클라이언트와 통신할 수 있도록 클라이언트 소켓(c_socket)을 반환함.
		printf("/client is connected\n");
		numClient++;
		printf("현재 접속 중인 클라이언트 수 :  [%d]\n", numClient);
		if((pid = fork()) > 0){//부모프로세스
			close(c_socket);
			continue;
		}else if(pid == 0){//자식프로세스
			close(s_socket);
			do_service(c_socket);
			exit(0);
		}else{//fork()합수 실패
			printf("fork() failed\n");
		}
	}
	return 0;	
}

void do_service(int c_socket){//5. 클라이언트 요청 처리
		while(1){
			for(i=0;i<BUFSIZE;i++){
				rcvBuffer[i]='\0';
				sendBuffer[i]='\0';}
			
			
			n = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[n]='\0'; //뒤에 붙는 개행문자삭제
			printf("received data : %s\n",rcvBuffer);
			
			if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer,"kill server",11) == 0)
				break;
			else if(strncasecmp(rcvBuffer, "안녕하세요",  5) == 0)
					write(c_socket, sendBuffer2, strlen(sendBuffer2));
			else if(strncasecmp(rcvBuffer, "이름이 뭐야?", 7) == 0)
					write(c_socket, sendBuffer3, strlen(sendBuffer3));
			else if(strncasecmp(rcvBuffer, "몇 살이야?" , 6) ==0)
					write(c_socket, sendBuffer4, strlen(sendBuffer4));
			else if(strncasecmp(rcvBuffer, "strlen", 6) == 0){
						lt = strlen(rcvBuffer)-7;
						sprintf(sendBuffer2, "글자 길이는 : %d",lt);
						write(c_socket, sendBuffer2, strlen(sendBuffer2));
			}
			else if(strncmp(rcvBuffer,"strcmp", 6) == 0){
				cnt = 0;
				t = strtok(rcvBuffer," ");
				if(t != NULL){
					while(t != NULL){
						t = strtok(NULL," ");
						if(t != NULL){
							strcpy(cmpBuffer[cnt],t);
							cnt++;
						}
					}
			}
			if(strcmp(cmpBuffer[0], cmpBuffer[1]) == 0){
				strcpy(sendBuffer,"0");
			}else{ strcpy(sendBuffer,"1" );
			}
		}
		else if(strncmp(rcvBuffer, "readfile", 8) == 0 ){
			t = strtok(rcvBuffer, " "); //t = readfile
			cnt = 0;
			while(t != NULL){
				strcpy(cmpBuffer[cnt], t);//cmpBuffer[0] = readfile, cmpBuffer[1] = 파일명 
				cnt++;
				t = strtok(NULL," "); //t = 파일명
			}
			/*// other
			 t = strtok(rcvBuffer, " "); //t = readfile
			 t = strtok(NULL, " "); //t = <filename> 
			 fp = fopen(t, "r");			
			*/
			fp = fopen(cmpBuffer[1], "r");
			if(fp){ //파일이 정상적으로 오픈되었다면 실행
				while(fgets(buff, 255, (FILE *)fp)){
					printf("%s", buff);
					write(c_socket, buff, strlen(buff));
				}
				fclose(fp);			
			}else{
				write(c_socket, sendBuffer5, strlen(sendBuffer5));
			}
			
		}
		else if(strncmp(rcvBuffer, "exec", 4) == 0 ){
			char *t;			
			char *command;
			t = strtok(rcvBuffer, " "); //t = exec
			command = strtok(NULL, "\0");//exec 뒤에있는 모든 문자열을 command 로 저장 "\0"은 공			
			printf("command is %s",command);
			int result = system(command);//command가 정상실행되면 0을 리턴, 그렇지 않으면 0이아닌 에러코드 리턴
			if(!result)//성공한 경우
				sprintf(buffer, "[%s] 명령어가 성공했습니다. n", command);
			else
				sprintf(buffer, "무슨 말인지 모르겠습니다.");
			
			write(c_socket, buffer, strlen(buffer));
			
		}
		else
			write(c_socket, sendBuffer5, strlen(sendBuffer5));	
		
		n=strlen(sendBuffer);
		write(c_socket, sendBuffer, n);
	}
		close(c_socket);
}
void sig_handler(int signo){
	int pid;
	int status;
	
	pid = wait(&status);
	printf("pid[%d] is terminated. status = %d\n", pid, status);//status = 정상적으로 종료되면 0 반환
	numClient--;
	printf("현재 접속 중인 클라이언트 수 :  [%d]\n",numClient);
}

