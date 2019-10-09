#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define BUFSIZE 100
char buffer[BUFSIZE] = "Hi, I am server";
char rcvBuffer[BUFSIZE];
char sendBuffer[BUFSIZE];
char sendBuffer2[BUFSIZE] = "안녕하세요, 만나서만가워요";
char sendBuffer3[BUFSIZE] = "내 이름은 이건우야";
char sendBuffer4[BUFSIZE] = "나는 23살이야";
char cmpBuffer[2][BUFSIZE];
char *t;

int main(){
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len;
	int n;
	int i;
	int lt;
	int cnt=0;
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

	//5. 클라이언트 요청 처리
	while(1){ //무한 루프
		len = sizeof(c_addr);
		printf("클라이언트 접속을 기다리는 중....\n");
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len); 
		//클라이언트의 요청이 오면 허용(accept)해 주고, 해당 클라이언트와 통신할 수 있도록 클라이언트 소켓(c_socket)을 반환함.
		printf("/client is connected\n");
		printf("클라이언트 접속 허용\n");
		while(1){
			for(i=0;i<BUFSIZE;i++){
				rcvBuffer[i]='\0';
				sendBuffer[i]='\0';}
			
			
			n = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
			rcvBuffer[n]='\0';
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
			}else{ strcpy(sendBuffer,"1" );}
		}
		n=strlen(sendBuffer);
		write(c_socket, sendBuffer, n);
	}
		close(c_socket);
		if(strncasecmp(rcvBuffer,"kill server",11) == 0)
			break;
	}
	close(s_socket);
	return 0;	
}
