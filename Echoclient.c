#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define PORT 9000
#define IPADDR "127.0.0.1"
#define BUFSIZE 100
char Buffer[100]="Hi,I'm client";//실습 3-4 서버에게 보낼 메시지
char buf1[100];//실습 3-6 fgets()를 이용해 문자열을 저장할 변수

int main(){
	int c_socket;
	struct sockaddr_in c_addr;
	int n;
	char rcvBuffer[BUFSIZE];//서버에서 보내준 메세지를 저장하는 변수
	//char sendBuffer[BUFSIZE] = "Hi I'm Buffer; //실습3-5 서버에 보낼 메시지
	//1. 클라이언트 소켓 생성
	c_socket = socket(PF_INET, SOCK_STREAM, 0);//서버와 동일한 설정으로 생성
	
	//2. 소켓 정보 초기화(어디로 접속할 건지)
	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr(IPADDR);
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(PORT);

	//3. 서버에 접속
	if(connect(c_socket, (struct sockaddr *)&c_addr, sizeof(c_addr)) == -1){
		//서버 접속에 실패하면		
		printf("Cannot Connect\n");// Cannot Connect 메시지 출력
		close(c_socket);//자원 회수
		return -1; //프로세스 종료
	}
	
	//write(c_socket, Buffer, strlen(Buffer));
	// 실습 3-4	서버에게 Buffer 내용을 전송
	//실습 3-5 서버에 메시지 보내기
	//wirte(c_socket, sendBuffer, strlen(sendBuffer));
	fgets(buf1, sizeof(buf1), stdin);//실습 3-6 fgets()를 이용해 문자열을 입력받고 buf1에 저장
	write(c_socket, buf1, strlen(buf1));//실습3-6 입력받은 문자가저장되어있는 buf1를 서버에게 전송

	//4. 서버에서 보낸 메시지 읽기
	n = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
	//서버에서 보내준 메세지를  rcvBuffer에 저장하고, 메세지의 길이를 n에 저장
	//만약 read에 실패하면  -1을 리턴
	if(n<0){ //read() 함수 실패시 작동
		printf("Read Failed\n");
		return -1;
	}
	rcvBuffer[n]='\0';
	printf("received data: %s\n", rcvBuffer); //서버에서 받은 메세지 출력
	close(c_socket);
	return 0;
}



