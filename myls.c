#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
char type(mode_t);
char *perm(mode_t);
void printStat(char*, char*, struct stat*);
int returnBlocks(char*, char*, struct stat*);
void modeA(char*);
void modeL(char*);
void modeAL(char*);
void modeD(char*);

int main(int argc, char **argv) {
	char *dir;
	
	if (argc == 1) { // 인자 없이 기본으로 실행했을때
		dir = ".";
		modeD(dir);
	}
	else {
		if (strcmp(argv[1], "-a") == 0) { // -a 옵션을 주면 실행
			if (argc == 2)
				dir = ".";
			else
				dir = argv[2];
			modeA(dir);
		}
		else if (strcmp(argv[1], "-l") == 0) { // -l 옵션을 주면 실행
			if (argc == 2)
				dir = ".";
			else
				dir = argv[2];
			modeL(dir);
		}
		else if (strcmp(argv[1], "-al") == 0) { // -al 옵션을 주면 실행
			if (argc == 2)
				dir = ".";
			else
				dir = argv[2];
			modeAL(dir);
		}	
		else { // 인자가 2개 이상이고 옵션을 주지 않으면 파일 경로이기 때문에 디렉토리에 주고 실행
			dir = argv[1];
			modeD(dir);
		}
	}

	
}

void modeA(char *dir) {
	DIR *dp;
	struct dirent *d;
	struct stat st;
	char path[BUFSIZ+1];
	
	if ((dp = opendir(dir)) == NULL)
		perror(dir);

	while ((d = readdir(dp)) != NULL) {
		printf("%s  ", d->d_name);
	}
	
	printf("\n");
	closedir(dp);
	exit(0);
}

void modeL(char *dir) {
	DIR *dp;
	DIR *dp2;
	struct dirent *d;
	struct stat st;
	char path[BUFSIZ+1];
	int sum = 0;
	
	if ((dp = opendir(dir)) == NULL)
		perror(dir);
	if ((dp2 = opendir(dir)) == NULL)
		perror(dir);
	
	while ((d = readdir(dp2)) != NULL) {
		if (d->d_name[0] != '.') { // 숨김 파일은 표시 안함
			sprintf(path, "%s/%s", dir, d->d_name);
			if (lstat(path, &st) < 0)
				perror(path);
			sum += returnBlocks(path, d->d_name, &st);
		}
	}
	printf("합계 %d\n", sum / 2);

	while ((d = readdir(dp)) != NULL) {
		if (d->d_name[0] != '.') { // 숨김 파일은 표시 안함
			sprintf(path, "%s/%s", dir, d->d_name);
			if (lstat(path, &st) < 0)
				perror(path);
			printStat(path, d->d_name, &st);
			putchar('\n');
		}
	}
	
	closedir(dp);
	closedir(dp2);
	exit(0);
}

void modeAL(char *dir) {
	DIR *dp;
	DIR *dp2;
	struct dirent *d;
	struct stat st;
	char path[BUFSIZ+1];
	int sum = 0;
	
	if ((dp = opendir(dir)) == NULL)
		perror(dir);
	if ((dp2 = opendir(dir)) == NULL)
		perror(dir);
	
	while ((d = readdir(dp2)) != NULL) {
		sprintf(path, "%s/%s", dir, d->d_name);
		if (lstat(path, &st) < 0)
			perror(path);
		sum += returnBlocks(path, d->d_name, &st);
	}
	printf("합계 %d\n", sum / 2);

	while ((d = readdir(dp)) != NULL) {
		sprintf(path, "%s/%s", dir, d->d_name);
		if (lstat(path, &st) < 0)
			perror(path);
		printStat(path, d->d_name, &st);
		putchar('\n');
	}
	
	closedir(dp);
	closedir(dp2);
	exit(0);

}

void modeD(char *dir) {
	DIR *dp;
	struct dirent *d;
	struct stat st;
	char path[BUFSIZ+1];
	
	if ((dp = opendir(dir)) == NULL)
		perror(dir);

	while ((d = readdir(dp)) != NULL) {
		if (d->d_name[0] != '.') // 숨김 파일은 표시 안함
			printf("%s  ", d->d_name);
	}
	printf("\n");
	closedir(dp);
	exit(0);
}

int returnBlocks(char *pathname, char *file, struct stat *st) { // 블록 합계 
	return st->st_blocks;
}

void printStat(char *pathname, char *file, struct stat *st) {
	printf("%c%s ", type(st->st_mode), perm(st->st_mode));
	printf("%3ld ", st->st_nlink);
	printf("%s %s ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);
	printf("%9ld ", st->st_size);
	printf("%.12s ", ctime(&st->st_mtime)+4);
	printf("%s", file);
}

char type(mode_t mode) {
	if (S_ISREG(mode))
		return('-');
	if (S_ISDIR(mode))
		return('d');
	if (S_ISCHR(mode))
		return('c');
	if (S_ISBLK(mode))
		return('b');
	if (S_ISLNK(mode))
		return('l');
	if (S_ISFIFO(mode))
		return('p');
	if (S_ISSOCK(mode))
		return('s');
}

char* perm(mode_t mode) {
	int i;
	static char perms[10];
	
	strcpy(perms, "---------");
	for (i=0; i<3; i++) {
		if (mode & (S_IREAD >> i*3))
			perms[i*3] = 'r';
		if (mode & (S_IWRITE >> i*3))
			perms[i*3+1] = 'w';
		if (mode & (S_IEXEC >> i*3))
			perms[i*3+2] = 'x';
	}
	
	return(perms);
}
