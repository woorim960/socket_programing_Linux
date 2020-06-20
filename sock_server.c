/*
===========================================================================
Name        : socket_server
Author      : Park Woo Rim
Version     : v1.0
Copyright   : Park Woo Rim
Description : 로그 파일의 데이터를 최신 것 부터 보여준다.

구분    날짜        내용
----   ---------- -----------------------------------------------------------
생성    2020/05/18 로그 출력 프로그램 생성 : main()

코딩구분 설명
생성 : 처음으로 프로그램 개발했을 때
추가 : 기존의 프로그램에서 알고리즘(로직)을 추가했을 때, 함수 추가
수정 : 기존의 프로그램의 알고리즘(로직)을 수정했을 때
삭제 : 기존의 프로그램 알고리즘 일부를 삭제했을 때
===========================================================================
*/

// 헤더 선언
#include "headers/init.h"
#include "headers/structures.h"
#include "headers/functions.h"
#include <time.h>
#include <signal.h>

void call_sig();

// 메인 함수 시작 => 메인 함수에서는 추후 프로그램의 크기가 커질 것을 감안하여 전체적인 프로그램의 진행에 대해서만 알 수 있도록 구성하였음.
int main(int argc, char *argv[]) {
  
  // 구조체 선언
  struct sockaddr_in serv_addr, clnt_addr;
  RECEIVE_RECORD msg;

  // 변수 선언
  int sock_fd, clnt_fd, file_fd;
  char log_file[ENV_VALUE_SIZE], log_path[ENV_VALUE_SIZE]; // 로그가 기록되어 있는 파일(writelog.txt)의 이름이 저장될 변수
  char exit_msg[MASSAGE_SIZE];
  socklen_t addr_len = sizeof(clnt_addr);

  // 프로그램 실행시 인자(포트번호)를 입력하지 않으면 실행
  if (argc != 2) {
    printf("사용법 : %s [PORT]\n", argv[0]);
    exit(0);
  }

  // IPv4, TCP로 소켓 생성
  if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    puts("서버 소켓 생성 에러(1)!");
    exit(1);
  }

  // 서버 주소체계 설정
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[1]));
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // 열린 소켓에 설정된 주소체계를 바인딩(결속, 묶다)
  if (bind(sock_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
    puts("서버 소켓 바인드 에러(1)!");
    exit(1);
  }

  // 클라이언트 접속 대기
  if (listen(sock_fd, 5) == -1) {
    puts("서버 소켓 Listen 에러(1)!");
    exit(1);
  }

  // 로그가 저장될 경로 설정
  set_logfile(log_file, log_path);

  // 로그가 저장될 파일 오픈
  if ((file_fd = open(log_path, O_WRONLY | O_CREAT | O_APPEND)) == -1) {
    puts("파일이 존재하지 않습니다.");
    exit(0);
  }

  // 서버 오픈 로그 저장
  save_log("서버 오픈", msg, file_fd, argv[1]);

  system("clear");

  signal(SIGALRM, call_sig);
  
  // "다수의 클라이언트 연결"을 허용하기 위한 자식프로세스 생성
  if (fork() == 0) {
    while(1) {
      // 클라이언트 접속 연결 허용
      if ((clnt_fd = accept(sock_fd, (struct sockaddr*) &clnt_addr, &addr_len)) == -1) {
        puts("서버 소켓에서 클라이언트 소켓 생성 에러(1)!");
        exit(1);
      } else {
        if (read(clnt_fd, &msg, sizeof(msg)) == -1) {
          puts("클라이언트가 전달한 접속 연결 메세지 읽기 에러");
          exit(0);
        };
        if (write(file_fd, &msg, sizeof(msg)) == -1) {
          puts("클라이언트 접속 연결 로그 저장 에러");
          exit(0);
        }
      }

      // 자식 프로세스 생성 후 데이터 가공 및 응답처리
      if (fork() == 0) {

        // 클라이언트 요청 및 응답 처리
        while(1) {
          // 클라이언트의 요청 데이터 읽기
          if (read(clnt_fd, &msg, sizeof(msg)) > 0) {

            // 클라이언트에서 입력한 메세지가 "exit"이면 클라이언트와의 연결 해제
            if (!strncmp(msg.message, "exit", 4)) {
              write(clnt_fd, "exit", 4); // 클라이언트에게 "exit" 전송(클라이언트는 해당 메세지를 받은 후 종료된다.)
              read(clnt_fd, &msg, sizeof(msg)); // "클라이언트 연결 종료" 메시지 읽기
              write(file_fd, &msg, sizeof(msg)); // "클라이언트 연결 종료" 메시지 파일에 저장

              // 클라이언트 소켓 종료 후 자식프로세스 종료
              close(clnt_fd);
              exit(0);
              break;
            }

            // 클라이언트 메세지 파일에 저장
            write(file_fd, &msg, sizeof(msg));
          }
        }
      }
    }
  }

  // 프로그램 종료
  close(file_fd);
  close(sock_fd);
  exit(0);
}

// 부모 프로세스에게 SIGALRM 신호가 전달될 때 실행되어 부모 프로세스까지 종료되도록 하기 위함이다.
void call_sig() {
  exit(0);
}
