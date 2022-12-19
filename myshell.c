#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/signal.h>

#define SIZE 512

int inputList(char* argv, char* in[]) { // 입력받은 인자를 나누는 함수
	int argc = 0;
	char* input;

	if (argv == NULL)
		return argc;
		
	input = strtok(argv, " \t\n");
	
	while (input != NULL) { // 입력받은 문자열 끝까지 반복
		in[argc] = input;
		input = strtok(NULL, " \t\n");
		argc++;
	}
	in[argc] = '\0';
	return argc;
}

int main() {
	int fd, pid, status;
	int idx, pipeidx; //파이프 관련
	int filedes[2]; // 파이프 관련
	int argc = -1, amper = -1; // 후면처리 변수
	char *fname;
	char *cmd[SIZE], *cmd1[SIZE], *cmd2[SIZE]; 
	char argv[SIZE], tempstr[SIZE], tempargv[SIZE];	
	
	while (1) {	
		printf("MyShell: ");
		fgets(argv, sizeof(argv), stdin);
		int argvlen = strlen(argv);
		argv[argvlen - 1] = '\0';
		strcpy(tempargv, argv);
		
		if (strlen(argv))
			strncpy(tempstr, argv, strlen(argv) - 2);
			
		argc = inputList(tempargv, cmd);
		
		if (argc > 0) { // 명령어가 있으면
			if (strcmp(cmd[0], "exit") == 0 || strcmp(cmd[0], "logout") == 0)
				exit(0); // exit, logout 입력 시 종료

			if (strcmp(cmd[argc - 1], "&") == 0) { // 후면처리
				amper = 1;
				cmd[argc - 1] = '\0';
				argc--;
			}
			else // 전면처리
				amper = 0;	
			
			pid = fork();
			
			if (pid == 0) { // 자식 프로세스
				if (strchr(argv, '|') != NULL) {	// 파이프
					for(int i = 0; i < argc; i++) {
						if (strcmp(cmd[i], "|") == 0)
							pipeidx = i;
					}
					for (int i = 0; i < pipeidx; i++)
						cmd1[i] = cmd[i];
					for(int i = 0; i < argc - (pipeidx + 1); i++)
						cmd2[i] = cmd[pipeidx + 1];

					pipe(filedes);
						
					if (fork() == 0) { // 손자 프로세스
						close(filedes[0]);
						dup2(filedes[1], STDOUT_FILENO);
						close(filedes[1]);
						execvp(cmd1[0], cmd1);
						perror("pipe");
						exit(1);
					}
					else { // 자식 프로세스
						close(filedes[1]);
						dup2(filedes[0], STDIN_FILENO);
						close(filedes[0]);
						execvp(cmd2[0], cmd2);
						perror("pipe");
						exit(1);
					}
				}
				else if (strchr(argv, '<') != NULL && strchr(argv, '>') == NULL) { // 리다이렉션 입력
					for (int i = 0; i < argc; i++) {
						if (strcmp(cmd[i], "<") == 0) {
							idx = i;
							fname = cmd[idx + 1];
							break;
						}
					}
					if ((fd = open(fname, O_RDONLY)) == -1) { // 파일을 읽기 전용으로 열기
						perror("read error");
						exit(1);
					}
					close(STDIN_FILENO);
					dup(fd);
					close(fd);
					for (int i = 0; i < idx; i++)
						cmd1[i] = cmd[i];
					execvp(cmd1[0], cmd1);
					perror("redirect error");
					exit(1);
				}
				else if (strchr(argv, '>') != NULL && strchr(argv, '<') == NULL) { // 리다이렉션 출력
					for (int i = 0; i < argc; i++) {
						if (strcmp(cmd[i], ">") == 0) {
							idx = i;
							fname = cmd[idx + 1];
							break;
						}
					}
					if ((fd = creat(fname, 0600)) == -1) { // 없으면 새 파일 생성
						perror("can't write file");
						exit(1);
					}
					close(STDOUT_FILENO);
					dup(fd);
					close(fd);
					for (int i = 0; i < idx; i++)
						cmd1[i] = cmd[i];
					execvp(cmd1[0], cmd1);
					perror("redirect error");
					exit(1);
				}
				else { // 일반 명령 
					execvp(cmd[0], &cmd[0]);
					perror(cmd[0]);
					exit(1);
				}
			}
			else { // 부모 프로세스
				if (amper == 0)
					pid = wait(&status);
				else if (amper > 0) // 후면처리시 기다리지 않고 종료
					waitpid(-1, &status, WNOHANG);
			}
		}
	}
}
