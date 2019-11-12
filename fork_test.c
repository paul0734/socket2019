#include <stdio.h>

int main(){
	int a = 10;
	int b = 100;
	
	a = a + 10;

	int pid = fork();
	//fork() 함수는 현재 구동 중인 프로세스의 복제본을 생성
	//현재 프로세스 = 부모 프로세스
	//fork() 에 의해 생성된 프로세스 = 자식 프로세스
	//리턴 값(pid) = 0, 자식프로세스
	//리턴 값 = 자식프로세스의 pid값 if 부모프로세스
	//만약 fork() 함수 실패 시, -1 값을 리턴
	
	if(pid > 0){ //부모프로세스인경우
		printf("부모프로세스\n");
		a = a + 10;
		printf("[Parent] a = %d, b = %d\n", a, b);
		sleep(10);//프로그램을 종료시키지 않고 10초간 잠깐 정지
	}else if(pid == 0){//자식프로세스인 경우
		printf("자식프로세스\n");
		b = b * 10;
		printf("[Child] a = %d, b = %d\n", a, b);
		sleep(10);
	}else{//fork()함수 실패
		printf("fork()함수 실패\n");
	}
	return 0;
}
