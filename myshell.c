#include "myshell.h"

int main(int argc, char* argv[]) {
	char* lines[SIZE]; // 총 명령어 배열
	char* history[SIZE]; //history용 
	int cur = 0;
	char instr[10]; //입력받은 명령어
	char* path = "/bin/";

	seth(history);
	pid_t pid;
	int status;
	int count; //총 명령어 개수
	while (1) {
		printf("12193075_shell$ ");
		fgets(instr, SIZE - 1, stdin);
		if (instr[strlen(instr) - 1] == '\n')
			instr[strlen(instr) - 1] = '\0';
		lines[cur] = strdup(instr);
		history[cur] = strdup(instr);
		count = cur;
		cur = cur + 1; //history용
	
		//내장형 명령어
		if (strcmp(lines[count], "quit") == 0) { //quit
			printf("myshell developed by \"이유진(12193075)\" \n\n");
			break;
		}

		else if (strcmp(lines[count], "history") == 0) { //history
			historyp(history, cur);
		}
		else if (strcmp(lines[count], "help") == 0) { //help

			printf("NAME \n	myshell \n");
			printf("DESCRIPTION \n    myshell is simple shell which interact by inputting commands.It was made using forkand execvp.There are some bulit - in commands, quit, historyand help. When users input “& ” behind command, it run the process in the background.\n");
		}


		else { //내장형 명령어 이외의 경우
			char fullpath[20];
			char* tok;
			tok = strtok(instr, " ");
			int n = 0;
			while (tok != NULL) { //토큰 나누기
				lines[n++] = tok;
				tok = strtok(NULL, " ");
			}
			lines[n] = (char*)0;
			argc = n;

			strcpy(fullpath, path);
			strcat(fullpath, lines[0]); //fullpath 완성

			for (n = 0; n < strlen(fullpath); n++) {
				if (&fullpath[n] == "\n") fullpath[n] = '\0';
			}

			pid = fork();
			int i = 0;
			while (lines[i] != NULL) { //백그라운드 실행
				if (strcmp(lines[i], "&") == 0) {
					background(lines, i);
				}
				i++;
			}
			if (pid > 0) { //parent
				pid = wait(&status);
			}
			else if (pid == 0) { //child
				execvp(fullpath, lines);
				perror("execvp error");
				exit(-1);
			}
			else {
				perror("write error");
				exit(-11);
			}
			printf("\n");
		}
	}

}
void seth(char** hist) { // history 세팅
	for (int i = 0; i < 10; ++i) hist[i] = NULL;
}
void historyp(char** hist, int c) { //history 출력
	int i; int n = 1;
	for (i = c - 1; i >= 0; i--) {
		printf("%d) %s\n", n, hist[i]);
		n++;

		if (n > 10) break;
	}
	printf("\n");
}
void background(char** cmd, int i) { //백그라운드 실행
	cmd[i] = NULL;
	int pid2;
	pid2 = fork();
	if (pid2 > 0) exit(1); //기다리지 않음
	else if (pid2 == 0) {
		execvp(cmd[0], cmd);
	}
	else {
		perror("error");
		exit(-11);
	}
}

