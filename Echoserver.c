#include <stdio.h>



#include <netinet/in.h>



#include <sys/socket.h>



#include <string.h>







#define PORT 9000







char buffer[100] = "Hi, I'm server";

char buffer2[100]; //실습 3-4





main()



{



int c_socket, s_socket;



struct sockaddr_in s_addr, c_addr;



int len;



int n;







// 1. 서버 소켓 생성



//서버소켓 =클라이언트의 접속요청을 처리(허용)해주기 위한 소켓



s_socket=socket(PF_INET, SOCK_STREAM, 0);//TCP/IP 통신을 위한 서버 소켓 생성







//2.서버 소켓 주소 설정



memset(&s_addr, 0, sizeof(s_addr));//s_addr의 값을 모두 0으로 초기화



s_addr.sin_addr.s_addr = htonl(INADDR_ANY);//IP 주소 설정



s_addr.sin_family=AF_INET;



s_addr.sin_port=htons(PORT);











//3.  서버 소켓 바인딩



if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr))==-1){



	//바인드 작업 실패 시, Cannot Bind 메시지 출력 후 프로그램 종료



	printf("Can not Bind\n");



	return -1;



}







//4. listen()함수 실행



if(listen(s_socket, 5)==-1){



	//listen 작업 실패 시   listen Fail 메시지 출력 후 프로그램 종료



	printf("listen Fail\n");



	return -1;



}







//5. 클라이언트 요청 처리



// 요청을 허용한 후,  Hello World 메시지를 전송함



while(1){//무한 루프







	len=sizeof(c_addr);



	printf("클라이언트 접속을 기다리는중...\n");



	c_socket=accept(s_socket, (struct sockaddr *)&c_addr, &len);



	//클라이언트의 요청이오면 허용(accept)해 주고, 해당 클라이언트와 통신할 수 있도록 클라이언트 소켓(c_socket)을 반환함.



	printf("클라이언트 접속 허용\n");







	n = strlen(buffer);



	read(c_socket, buffer2, sizeof(buffer2));

	//실습3-4,3-6  클라이언트에게 받은 내용을 buffer2에 저장



	printf("%s\n", buffer2);//클라이언트에게 받은  buffer2내용 출력

	write(c_socket, buffer2, strlen(buffer2));//실습 3-6 입력받은 문자가 저장되어 있는 buffer2를 다시 전송



	//write(c_socket, buffer, n);

	//실습 3-4 클라이언트에게 buffer 내용 전송



	//write(c_socket, buffer, n);

	//클라이언트에게  buffer 의 내용을 전송함







	close(c_socket);



	}



close(s_socket);



return 0;



}





//1  gcc -o 원하는파일이름 HelloworldServer.c

//2  ./실행파일이름

//3  실행법 : 다른터미널에서  telnet localhost 포트번호


































































