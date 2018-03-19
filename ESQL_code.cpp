// 헤더 선언
#include <stdio.h>
#include <string.h>

// 상수 선언
#define MAX_STRING 32
#define INPUT_FILE_NAME "input.txt"
#define OUTPUT_FILE_NAME "output.txt"

EXEC SQL INCLUDE SQLCA.H;

EXEC SQL BEGIN DECLARE SECTION;

	char username[20];
	char password[20];

	//host variable
	char CID[15];
	char CNAME[15];
	char CADDR[30];
	char CNUM[30];

	char LID[15];
	char LNAME[30];
	char LADDR[30];
	char LNUM[30];

	char BID[15];
	char BNAME[30];
	char AUTHOR[30];
	char TYPE[30];
	int YEAR;
	int PAGE;

	char PID[15];
	char PNAME[30];
	char PADDR[30];
	char PNUM[30];

	char ANUM[15];

    char result_id[15];

	int cnt;

EXEC SQL END DECLARE SECTION;

// 함수 선언
void join();
void doTask();

// 변수 선언
FILE* in_fp, *out_fp;
int i=1;

int main()
{
	// 파일 입출력을 위한 초기화
	in_fp = fopen(INPUT_FILE_NAME, "r+");
	out_fp = fopen(OUTPUT_FILE_NAME, "w+");

	strcpy(username, "dbteam");
	strcpy(password, "abcd");
	EXEC SQL CONNECT :username
	IDENTIFIED BY :password;

	EXEC SQL CREATE TABLE customer
	(
		cid varchar2(15) primary key,
		cname varchar2(15) not null,
		caddr varchar2(30),
		cnum varchar2(30)
	);

	EXEC SQL CREATE TABLE library
	(
		lid varchar2(15) primary key,
		lname varchar2(30) not null,
		laddr varchar2(30),
		lnum varchar2(30)
	);

	EXEC SQL CREATE TABLE books
	(
		bid varchar2(15) primary key,
		bname varchar2(30) not  null,
		author varchar2(30),
		type varchar2(30),
		year number(4),
		page number(4),
		lid varchar2(15),
		FOREIGN KEY (lid) REFERENCES  library
	);

	EXEC SQL CREATE TABLE librarian
	(
		pid varchar2(15) primary key,
		pname varchar2(30) not null,
		paddr varchar2(30),
		pnum varchar2(30),
		lid varchar2(15),
		FOREIGN KEY (lid) REFERENCES library
	);

	EXEC SQL CREATE TABLE appointment
	(
		cid varchar2(15),
		bid varchar2(15),
		lid varchar2(15),
		anum varchar2(15),
		FOREIGN KEY (cid) REFERENCES customer,
		FOREIGN KEY (bid) REFERENCES books,
		FOREIGN KEY (lid)	REFERENCES library
	);

	printf("==================================\n");			//fprintf로 할 필요는 없는?
	printf("1.1. 회원가입\n 1.2. 로그인\n 1.3. 종료\n");
	doTask();

	EXEC SQL DROP TABLE appointment;
	EXEC SQL DROP TABLE librarian;
	EXEC SQL DROP TABLE books;
	EXEC SQL DROP TABLE library;
	EXEC SQL DROP TABLE customer;

	EXEC SQL COMMIT WORK RELEASE;
	return 0;
}

void doTask()
{
	// 메뉴 파싱을 위한 level 구분을 위한 변수
	int menu_level_1 = 0, menu_level_2 = 0;

	char t_anum[15];
    char input_id[15];


	// 종료 메뉴(1.3)가 입력되기 전까지 반복함
	while(menu_level_1 != 1 || menu_level_2 != 3)
	{
		// 입력파일에서 메뉴 숫자 2개를 읽기
		fscanf(in_fp, "%d %d ", &menu_level_1, &menu_level_2);

		// 메뉴 구분 및 해당 연산 수행
		switch(menu_level_1)
		{
		case 1:
			{
				switch(menu_level_2)
				{
				case 1:   // "1.1. 회원가입“ 메뉴 부분
					{
						// join() 함수에서 해당 기능 수행
						join();
						break;
					}
				 case 2:	//1.2 로그인 메뉴 부분
					{
						fscanf(in_fp, "%s", input_id);
						strcpy(CID, input_id);

						EXEC SQL SELECT COUNT(*) AS cnt
						INTO :cnt
				  		FROM customer
				   		WHERE cid = :CID;

			 		/*	EXEC SQL SELECT cid INTO :result_id
						FROM customer
						WHERE cid = :CID; */

						if(strcmp(input_id, "admin") == 0 && cnt == 1){
							fprintf(out_fp, "1.2. 로그인\n");
							fprintf(out_fp, "> %s\n", result_id);
							printf("==================================\n");
							printf("3.1. 도서관 정보 등록\n3.2. 도서관 정보 삭제\n3.3. 도서 정보 등록\n3.4. 도서 정보 삭제\n3.5. 사서 정보 등록\n3.6. 사서 정보 삭제\n3.7. 도서관 정보 전체 조회\n3.8. 도서관 정보 이름 조회\n3.9. 사서 정보 전체 조회\n3.10. 예약 내역 전체 조회\n3.11. 예약 내역 도서이름 조회\n3.12. 예약 내역 고객이름 조회\n3.13. 로그아웃\n");
						}
						
						else if(cnt == 1){
							fprintf(out_fp, "1.2. 로그인\n");
							fprintf(out_fp, "> %s\n", result_id);
							printf("==================================\n");
							printf("2.1. 회원 정보 조회\n2.2. 도서 예약\n2.3. 도서 예약 조회\n2.4. 도서 예약 삭제\n2.5. 회원탈퇴\n2.6. 로그아웃\n");
							}
						else{
		
							fprintf(out_fp, "일치하는 id가 없습니다.\n");
							printf("==================================\n");
							printf("1.1. 회원가입\n 1.2. 로그인\n 1.3. 종료\n");
						}
                        break;
                    }
				case 3:
					{
						fprintf(out_fp, "1.3. 종료\n");
						return 0;
					}
				}
				break;
			}
		case 2:
			{
				switch(menu_level_2)
				{
				case 1:   // "2.1 회원 정보 조회“ 메뉴 부분
					{
						EXEC SQL SELECT cname, caddr, cnum INTO :CNAME, :CADDR, :CNUM
						FROM customer
						WHERE cid = :CID;

						fprintf(out_fp, "2.1. 회원 정보 조회\n");
						fprintf(out_fp, "> %s %s %s %s\n", CID, CNAME, CADDR, CNUM);

						break;
					}
				case 2:	//2.2 도서 예약 메뉴 부분
					{
						fscanf(in_fp, "%s", BNAME);

						sprintf(t_anum, "R%03d", i);
						strcpy(ANUM, t_anum);

						EXEC SQL SELECT bid, bname, author, type, year, page, lid
						INTO :BID, :BNAME, :AUTHOR, :TYPE, :YEAR, :PAGE, :LID
						FROM books
						WHERE bname = :BNAME;

						EXEC SQL INSERT INTO appointment VALUES
						(:CID, :BID, :LID, :ANUM);
						i++;

						fprintf(out_fp, "2.2. 도서 예약\n");
						fprintf(out_fp, "> %s %s %s %s %d %d\n", ANUM, BNAME, AUTHOR, TYPE, YEAR, PAGE);

						break;
					}
				case 3:	// 2.3 도서 예약 조회 메뉴 부분
					{
						EXEC SQL DECLARE c1 CURSOR FOR
						SELECT a.anum, b.bname, b.author, b.type, b.year, b.page
						FROM books b, appointment a
						WHERE a.cid=:CID and b.bid=a.bid;

						fprintf(out_fp, "2.3. 도서 예약 조회\n");

						EXEC SQL OPEN c1;
						for(;;)
						{
							EXEC SQL FETCH c1 INTO :ANUM, :BNAME, :AUTHOR, :TYPE, :YEAR, :PAGE;
							if(sqlca.sqlcode != 0)
							{
								if(sqlca.sqlcode != 1403)
								{
									printf("Error Occured : %d\n", sqlca.sqlcode);
									EXEC SQL ROLLBACK RELEASE;
									return 0;
								}
								else
								{
									break;
								}
							}
							fprintf(out_fp, "> %s %s %s %s %d %d\n", ANUM, BNAME, AUTHOR, TYPE, YEAR, PAGE);
						}

						EXEC SQL CLOSE c1;

						break;
					}
				case 4:	// 2.4 도서 예약 삭제 메뉴 부분
					{
						fscanf(in_fp, "%s", ANUM);

						EXEC SQL DELETE FROM appointment
						WHERE anum = :ANUM;

						// 출력 형식
						fprintf(out_fp, "2.4. 도서 예약 삭제\n");
						fprintf(out_fp, "> %s\n", ANUM);

						break;
					}
				case 5:	// 2.5 회원탈퇴 메뉴 부분
					{
						// 출력 형식
						fprintf(out_fp, "2.5. 회원탈퇴\n");
						fprintf(out_fp, "> %s\n", CID);

						EXEC SQL DELETE FROM customer
						WHERE cid = :CID;

						break;
					}
				case 6:
					{
						fprintf(out_fp, "2.6. 로그아웃\n");
						printf("==================================\n");
						printf("1.1. 회원가입\n 1.2. 로그인\n 1.3. 종료\n");
						break;
					}
				}
				break;
			}
		case 3:
			{
				switch (menu_level_2)
				{
				case 1:		 // "3.1 도서관 정보 등록“ 메뉴 부분
					{

						fscanf(in_fp, "%s %s %s %s", LID, LNAME, LADDR, LNUM);

						EXEC SQL INSERT INTO library VALUES(:LID, :LNAME, :LADDR, :LNUM);

						// 출력 형식
						fprintf(out_fp, "3.1. 도서관 정보 등록\n");
						fprintf(out_fp, "> %s %s %s %s\n", LID, LNAME, LADDR, LNUM);
						break;
					}
				case 2:		 // "3.2 도서관 정보 삭제“ 메뉴 부분
					{
						fscanf(in_fp, "%s", LID);

						EXEC SQL DELETE FROM library
						WHERE lid = :LID;

						// 출력 형식
						fprintf(out_fp, "3.2. 도서관 정보 삭제\n");
						fprintf(out_fp, "> %s\n", LID);
						break;
					}
				case 3:         // "3.3 도서 정보  등록" 메뉴 부분
                    {
                        fscanf(in_fp, "%s %s %s %s %d %d %s", BID, BNAME, AUTHOR, TYPE, YEAR, PAGE, LID);
 
                        EXEC SQL INSERT INTO books VALUES
                        (:BID, :BNAME, :AUTHOR, :TYPE, :YEAR, :PAGE, :LID);
 
                        // 출력 형식
                        fprintf(out_fp, "3.3. 도서 정보 등록\n");
                        fprintf(out_fp, "> %s %s %s %s %d %d %s\n", BID, BNAME, AUTHOR, TYPE, &YEAR, &PAGE, LID);
                        break;
                    }
				case 4:				// "3.4. 도서 정보 삭제" 메뉴 부분
					{
						fscanf(in_fp, "%s", BID);

						EXEC SQL DELETE FROM books
						WHERE bid = :BID;

						// 출력 형식
						fprintf(out_fp, "3.4. 도서 정보 삭제\n");
						fprintf(out_fp, "> %s\n", BID);
						break;
					}
				case 5:				// "3.5. 사서 정보 등록" 메뉴 부분
					{
						fscanf(in_fp, "%s %s %s %s %s", PID, PNAME, PADDR, PNUM, LID);

						EXEC SQL INSERT INTO librarian VALUES
						(:PID, :PNAME, :PADDR, :PNUM, :LID);

						// 출력 형식
						fprintf(out_fp, "3.5. 사서 정보 등록\n");
						fprintf(out_fp, "> %s %s %s %s %s\n", PID, PNAME, PADDR, PNUM, LID);
						break;
					}
				case 6:				// "3.6. 사서 정보 삭제" 메뉴 부분
					{
						fscanf(in_fp, "%s", PID);

						EXEC SQL DELETE FROM librarian
						WHERE pid = :PID;

						// 출력 형식
						fprintf(out_fp, "3.6. 사서 정보 삭제\n");
						fprintf(out_fp, "> %s\n", PID);
						break;
					}
				case 7:				// "3.7. 도서관 정보 전체 조회" 메뉴 부분
					{
						EXEC SQL DECLARE c2 CURSOR FOR
						SELECT lid, lname, laddr, lnum
						FROM library;

						fprintf(out_fp, "3.7 도서관 정보 전체 조회\n");

						EXEC SQL OPEN c2;
						for(;;)
						{
							EXEC SQL FETCH c2 INTO :LID, :LNAME, :LADDR, :LNUM;
							if(sqlca.sqlcode != 0)
							{
								if(sqlca.sqlcode != 1403)
								{
									printf("Error Occured : %d\n", sqlca.sqlcode);
									EXEC SQL ROLLBACK RELEASE;
									return 0;
								}
								else
								{
									break;
								}
							}
							fprintf(out_fp, "> %s %s %s %s\n", LID, LNAME, LADDR, LNUM);
						}

						EXEC SQL CLOSE c2;

						break;
					}
				case 8:				// "3.8. 도서관 정보 이름 조회" 메뉴 부분
					{
						fscanf(in_fp, "%s", LNAME);

						EXEC SQL SELECT lid, lname, laddr, lnum
						INTO :LID, :LNAME, :LADDR, :LNUM
						FROM library
						WHERE lname = :LNAME;

						fprintf(out_fp, "3.8. 도서관 정보 이름 조회\n");

						fprintf(out_fp, "> %s %s %s %s\n", LID, LNAME, LADDR, LNUM);

						break;
					}
				case 9:				// "3.9. 사서 정보 전체 조회" 메뉴 부분
					{
						EXEC SQL DECLARE c3 CURSOR FOR
						SELECT pid, pname, paddr, pnum, lid
						FROM librarian;

						fprintf(out_fp, "3.9. 사서 정보 전체 조회\n");

						EXEC SQL OPEN c3;
						for(;;)
						{
							EXEC SQL FETCH c3 INTO :PID, :PNAME, :PADDR, :PNUM, :LID;
							if(sqlca.sqlcode != 0)
							{
								if(sqlca.sqlcode != 1403)
								{
									printf("Error Occured : %d\n", sqlca.sqlcode);
									EXEC SQL ROLLBACK RELEASE;
									return 0;
								}
								else
								{
									break;
								}
							}
							fprintf(out_fp, "> %s %s %s %s %s\n", PID, PNAME, PADDR, PNUM, LID);
						}

						EXEC SQL CLOSE c3;

						break;
					}
				case 10:			// "3.10. 예약 내역 전체 조회" 메뉴 부분
					{
						EXEC SQL DECLARE c4 CURSOR FOR
						SELECT a.anum, a.cid, b.bname, b.author
						FROM appointment a, books b
						WHERE a.bid = b.bid;

						fprintf(out_fp, "3.10. 예약 내역 전체 조회\n");

						EXEC SQL OPEN c4;
						for(;;)
						{
							EXEC SQL FETCH c4 INTO :ANUM, :CID, :BNAME, :AUTHOR;
							if(sqlca.sqlcode != 0)
							{
								if(sqlca.sqlcode != 1403)
								{
									printf("Error Occured : %d\n", sqlca.sqlcode);
									EXEC SQL ROLLBACK RELEASE;
									return 0;
								}
								else
								{
									break;
								}
							}
							fprintf(out_fp, "> %s %s %s %s\n", ANUM, CID, BNAME, AUTHOR);
						}

						EXEC SQL CLOSE c4;

						break;
					}
				case 11:			// "3.11. 예약 내역 도서이름 조회" 메뉴 부분
					{
						fscanf(in_fp, "%s", BNAME);

						EXEC SQL DECLARE c5 CURSOR FOR
						SELECT a.anum, a.cid, b.bname, b.author
						FROM appointment a, books b
						WHERE a.bid = b.bid and b.bname = :BNAME;


						fprintf(out_fp, "3.11. 예약 내역 도서이름 조회\n");

						EXEC SQL OPEN c5;
						for(;;)
						{
							EXEC SQL FETCH c5 INTO :ANUM, :CID, :BNAME, :AUTHOR;
							if(sqlca.sqlcode != 0)
							{
								if(sqlca.sqlcode != 1403)
								{
									printf("Error Occured : %d\n", sqlca.sqlcode);
									EXEC SQL ROLLBACK RELEASE;
									return 0;
								}
								else
								{
									break;
								}
							}
							fprintf(out_fp, "> %s %s %s %s\n", ANUM, CID, BNAME, AUTHOR);
						}

						EXEC SQL CLOSE c5;

						break;
					}
				case 12:			// "3.12. 예약 내역 고객이름 조회" 메뉴 부분
					{
						fscanf(in_fp, "%s", CNAME);

						EXEC SQL DECLARE c6 CURSOR FOR
						SELECT a.anum, a.cid, b.bname, b.author
						FROM appointment a, books b, customer c
						where a.bid = b.bid and a.cid = c.cid and c.cname = :CNAME;

						fprintf(out_fp, "3.12 예약 내역 고객이름 조회\n");

						EXEC SQL OPEN c6;
						for(;;)
						{
							EXEC SQL FETCH c6 INTO :ANUM, :CID, :BNAME, :AUTHOR;
							if(sqlca.sqlcode != 0)
							{
								if(sqlca.sqlcode != 1403)
								{
									printf("Error Occured : %d\n", sqlca.sqlcode);
									EXEC SQL ROLLBACK RELEASE;
									return 0;
								}
								else
								{
									break;
								}
							}
							fprintf(out_fp, "> %s %s %s %s\n", ANUM, CID, BNAME, AUTHOR);
						}

						EXEC SQL CLOSE c6;

						break;
					}
				case 13:
					{
						fprintf(out_fp, "3.13 로그아웃\n");
						fprintf(out_fp, "> admin\n");

						printf("==================================\n");
						printf("1.1. 회원가입\n 1.2. 로그인\n 1.3. 종료\n");
						break;
					}
				}
				break;
			}
		}
	}
}

void join()
{
	char ID[MAX_STRING], name[MAX_STRING], gender[MAX_STRING], phone_num[MAX_STRING], address[MAX_STRING];
	int age;

	// 입력 형식 : 주민번호, 이름, 주소, 전화번호를 파일로부터 읽음
	fscanf(in_fp, "%s %s %s %s\n", ID, name, address, phone_num);

	// DB 연산을 포함하여 해당 기능 수행
	strcpy(CID, ID);
	strcpy(CNAME, name);
	strcpy(CADDR, address);
	strcpy(CNUM, phone_num);

	EXEC SQL INSERT INTO customer(cid, cname, caddr, cnum) VALUES
	(:CID, :CNAME, :CADDR, :CNUM);

	// 출력 형식
	fprintf(out_fp, "1.1. 회원가입\n");
	fprintf(out_fp, "%s %s %s %s\n", ID, name, address, phone_num);

	return 0;
}
