#include <stdio.h>
#include <string.h>

int main(){
	char buffer[100] = "strcmp hello hihi";
	char *token;
	char *str[3];
	int idx = 0;
	token = strtok (buffer, " "); //buffer의 문자열을 공백을 기준으로 구분
	while(token != NULL){ //null(끝)까지 반복
		str[idx] = token;
		printf("str[%d] = %s\n", idx, str[idx]); 
		idx++;		
		token = strtok(NULL, " ");
		
	}
	if(idx < 3)
		strcmp(buffer, "error");
	else if(!strcmp(str[1], str[2])) //같은 문자열이면
		sprintf(buffer, "%s와 %s는 같은문자열입니다.",str[1], str[2]);
	else
		sprintf(buffer,"%s와 %s는 다른문자열입니다.", str[1], str[2]);
	
	printf("%s \n", buffer);
	return 0;
}


