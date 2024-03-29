#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define IPADDR "127.0.0.1"
#define BUFSIZE 100

int main(){
	int c_socket; 
	struct sockaddr_in c_addr;
	int n;
	int i;
	char rcvBuffer[BUFSIZE];//서버에서 보내준 메세지를 저장하는 변수
	char sendBuffer[BUFSIZE];
	
	//1. 클라이언트 소켓 생성
	c_socket = socket(PF_INET, SOCK_STREAM, 0); //서버와 동일한 설정으로 생성
	//2.소켓 정보 초기화
	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR); //접속할 IP 설정 (127.0.0.1)
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);

	//3. 서버에 접속
	if(connect(c_socket, (struct sockaddr *) &c_addr, sizeof(c_addr)) == -1){
		//서버 접속에 실패하면 
		printf("Cannot Connect\n"); //Cannot connect 메세지 출력
		close(c_socket); //자원 회수
		return -1;  //프로세스 종료
	}
	//4. 서버에 메세지 보내기
	//입력받기
	while(1){
		printf("input message : ");
		fgets(sendBuffer, sizeof(sendBuffer), stdin);
		sendBuffer[strlen(sendBuffer)-1] = '\0';
	 	write(c_socket,sendBuffer,strlen(sendBuffer));
		if(strncasecmp(sendBuffer,"quit",4) == 0 || strncasecmp(sendBuffer,"kill server",11) == 0)
			break;
		n = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		
		if (n < 0){ //read() 함수 실패 
			printf("Read Failed\n");
			return -1;
		}
		rcvBuffer[n] = '\0';
		printf("Server : %s\n", rcvBuffer); 
		//서버에서 받은 메세지 출력
		for(i=0;i<BUFSIZE;i++)
			rcvBuffer[i] = '\0';
		//printf("rcvBuffer length: %d\n", n); 
		//3-2. 서버에서 받은 메세지의 길이 출력
		
		
	}
	close(c_socket);
	return 0;	
}
