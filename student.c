//***************************************************************
//                   HEADER FILE USED IN PROJECT
//***************************************************************

#include "getch.h"
#include <stdio.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
typedef struct _Student student; // 이름 재정의

//***************************************************************
//                   STURUCTURE USED IN PROJECT
//****************************************************************

struct _Student
{
	int rollno;
	char name[50];
	int p_marks, c_marks;
	double per;
	char grade;
};

//***************************************************************
//        function to write in file
//****************************************************************

void write_student()
{
	int fd;
	student st[1];
	if ((fd = open("student.txt", O_WRONLY)) == -1) { // 쓰기 
		perror("open");
		exit(1);
	}
	lseek(fd, 0L, SEEK_END); // 끝에서 부터 작성하기 위해 파일 포인터를 문서의 끝으로 보낸다 
	printf("\nPlease Enter The New Details of student \n");
	printf("\nEnter The roll number of student : ");
	scanf("%d", &st->rollno);
	getchar(); // flushing buffer
	printf("\n\nEnter The Name of student : ");
	fgets(st->name, sizeof(st->name), stdin);
	st->name[strlen(st->name) - 1] = '\0'; // fgets 사용 시 끝에 강제로 개행문자가 들어가기 때문에 출력 시 줄바뀜 방지
	printf("\nEnter The marks in physics out of 100 : ");
	scanf("%d", &st->p_marks);
	printf("\nEnter The marks in chemistry out of 100 : ");
	scanf("%d", &st->c_marks);

	st->per = (st->p_marks + st->c_marks) / 2.0;
	if (st->per >= 60)
		st->grade = 'A';
	else if (st->per >= 50 && st->per < 60)
		st->grade = 'B';
	else if (st->per >= 33 && st->per < 50)
		st->grade = 'C';
	else
		st->grade = 'F';
	write(fd, st, sizeof(st));
	close(fd);
	getchar(); // flushing buffer
	printf("\n\nStudent Record Has Been Created. Press any key.... ");
	getch();
}


//***************************************************************
//        function to read all records from file
//****************************************************************


void display_all()
{
	int fd;
	ssize_t nread;
	student st[1];
	if ((fd = open("student.txt", O_RDONLY)) == -1) {
		perror("open");
		exit(1);
	}
	system("clear"); // display clear
	printf("\n\n\n\t\tDISPLAY ALL RECORD !!!\n\n");
	printf("====================================================\n");
	printf("R.No.  Name       P   C   Ave    Grade\n");
	printf("====================================================\n");
	
	while ((nread = read(fd, st, sizeof(student))) > 0)
	{ // 파일 사이즈 만큼 반복하면서 모든 학생 레코드 
		printf("%-6d %-10s %-3d %-3d %-3.2f  %-1c\n",
			st->rollno, st->name, st->p_marks, st->c_marks, st->per, st->grade);
	}
	close(fd);
	getch();
}


//***************************************************************
//        function to read specific record from file
//****************************************************************


void display_sp(int n)
{
	int flag = 0;
	int fd;
	ssize_t nread;
	student st[1];
	if ((fd = open("student.txt", O_RDONLY)) == -1) {
		perror("open");
		exit(1);
	}
	while ((nread = read(fd, st, sizeof(student))) > 0)
	{
		if (st->rollno == n)
		{ // 해당하는 학생의 이름과 점수를 출력
			printf("\nRoll number of student : %d", st->rollno);
			printf("\nName of student : %s", st->name);
			printf("\nMarks in Physics : %d", st->p_marks);
			printf("\nMarks in Chemistry : %d", st->c_marks);
			printf("\nPercentage of student is : %.2f", st->per);
			printf("\nGrade of student is : %c", st->grade);
			flag = 1;
		}
	}
	close(fd);
	if (flag == 0) // 해당 학생이 없다면 검색 불가
		printf("\n\nRecord Not Exist");
	getch();
}


//***************************************************************
//        function to modify record of file
//****************************************************************


void modify_student()
{
	int no, found = 0, i;
	int fd;
	ssize_t nread;
	student st[sizeof(student)];
	if ((fd = open("student.txt", O_RDWR)) == -1) {
		perror("open");
		exit(1);
	}
	system("clear"); // display clear
	printf("\n\n\tTo Modify ");
	printf("\n\n\tPlease Enter The roll number of student : ");
	scanf("%d", &no);
	while ((nread = read(fd, st, sizeof(student))) > 0 && found == 0)
	{
		if (st->rollno == no)
		{ // 해당 학생이 있으면 변경
			printf("\nRoll number of student : %d", st->rollno);
			printf("\nName of student : %s", st->name);
			printf("\nMarks in Physics : %d", st->p_marks);
			printf("\nMarks in Chemistry : %d", st->c_marks);
			printf("\nPercentage of student is : %.2f", st->per);
			printf("\nGrade of student is : %c", st->grade);
			printf("\nPlease Enter The New Details of student \n");
			printf("\nEnter The roll number of student : ");
			scanf("%d", &st->rollno);
			getchar();  //flushing buffer
			printf("\n\nEnter The Name of student : ");
			fgets(st->name, sizeof(st->name), stdin);
			st->name[strlen(st->name) - 1] = '\0'; // fgets 사용 시 끝에 강제로 개행문자가 들어가기 때문에 출력 시 줄바뀜 방지
			printf("\nEnter The marks in physics out of 100 : ");
			scanf("%d", &st->p_marks);
			printf("\nEnter The marks in chemistry out of 100 : ");
			scanf("%d", &st->c_marks);

			st->per = (st->p_marks + st->c_marks) / 2.0;
			if (st->per >= 60)
				st->grade = 'A';
			else if (st->per >= 50 && st->per < 60)
				st->grade = 'B';
			else if (st->per >= 33 && st->per < 50)
				st->grade = 'C';
			else
				st->grade = 'F';

			lseek(fd, -sizeof(student), SEEK_CUR); // 바꾸려는 학생 레코드로 파일 포인터 이동
			write(fd, st, sizeof(student)); // 그 뒤 바꾸려는 내용으로 덮어쓴다
			printf("\n\n\t Record Updated");
			found = 1;
			break;
		}
	}
	close(fd);
	getchar(); // flushing buffer
	if (found == 0) // 학생이 없을 경우 변경 
		printf("\n\nRecord Not Found");
	getch();
}


//***************************************************************
//        function to delete record of file
//****************************************************************


void delete_student()
{
	int no;
	int flag = 0;
	int fd;
	int fd2;
	ssize_t nread;
	student st[sizeof(student)];
	if ((fd = open("student.txt", O_RDONLY)) == -1) {
		perror("open");
		exit(1);
	}
	system("clear"); // display clear
	printf("\n\n\n\tDelete Record");
	printf("\n\nPlease Enter The roll number of student You Want To Delete : ");
	scanf("%d", &no);
	while ((nread = read(fd, st, sizeof(student))) > 0) {
		if (st->rollno == no) { // 해당 학생이 있으면 삭제
			if ((fd2 = open("temp.txt", O_RDWR | O_CREAT | O_TRUNC, 0700)) == -1) {
				perror("open");
				exit(1);
			}
			while ((nread = read(fd, st, sizeof(student))) > 0) {
				if (st->rollno != no)
				{ // 삭제 대상이 아닌 학생 레코드만 임시 파일로 쓰기
					write(fd2, st, sizeof(student));
				}
			}
			flag = 1;
			remove("student.txt"); // 원본 파일을 지우고
			rename("temp.txt", "student.txt"); // 임시 파일을 원본 파일로 변경
			close(fd2);
		}
	}
	close(fd);
	getchar(); // flushing buffer
	if (flag == 0) // 해당 학생이 없다면 삭제 불가
		printf("\n\nRecord Not Found");
	else 
		printf("\n\n\tRecord Deleted..");
	getch();
}



//***************************************************************
//        THE MAIN FUNCTION OF PROGRAM
//****************************************************************
void main()
{
	int fd;
	if ((fd = open("student.txt", O_RDWR | O_CREAT | O_TRUNC, 0700)) == -1) {
		// 프로그램 실행 시 해당 파일이 없으면 생성하고, 있으면 오류 방지를 위해 내용을 초기화한다.
		perror("open");
		exit(1);
	}
	close(fd);
	char ch;
	int num;
	do
	{
		system("clear"); // display clear

		printf("\n\n\t1.CREATE STUDENT RECORD");
		printf("\n\n\t2.DISPLAY ALL STUDENTS RECORDS");
		printf("\n\n\t3.SEARCH STUDENT RECORD ");
		printf("\n\n\t4.MODIFY STUDENT RECORD");
		printf("\n\n\t5.DELETE STUDENT RECORD");
		printf("\n\n\t6.EXIT");
		printf("\n\n\tPlease Enter Your Choice (1-6) : ");
		ch = getch();

		switch (ch)
		{
		case '1':	system("clear"); // display clear
			write_student();
			break;
		case '2':	display_all();
			break;
		case '3':	system("clear"); // display clear
			printf("\n\n\tPlease Enter The roll number : ");
			scanf("%d", &num);
			getchar(); // flushing buffer
			display_sp(num);
			break;
		case '4':	modify_student(); break;
		case '5':	delete_student(); break;
		case '6':	break;
		default:	printf("\a");
		}
	} while (ch != '6');
}
//***************************************************************
//                END OF PROJECT
//***************************************************************
