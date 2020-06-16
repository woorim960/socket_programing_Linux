/*
===========================================================================
Name        : socket_client
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

// 메인 함수 시작 => 메인 함수에서는 추후 프로그램의 크기가 커질 것을 감안하여 전체적인 프로그램의 진행에 대해서만 알 수 있도록 구성하였음.
int main(int argc, char *argv[]) {
  
  // 구조체 선언
  struct sockaddr_in serv_addr;
  RECEIVE_RECORD msg;

  // 변수 선언
  int sock_fd;
  char exit_msg[4];
  char* ip_addr;

  // 프로그램 실행시 인자(포트번호)를 입력하지 않으면 실행
  if (argc != 3) {
    printf("사용법 : %s [IP] [PORT]\n", argv[0]);
    exit(0);
  }

  // IPv4, TCP로 소켓 생성
  if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
    puts("클라이언트 소켓 생성 에러(1)!");
    exit(1);
  }

  // 서버 주소체계 설정
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

  // 서버 소켓에 연결 요청
  if (connect(sock_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
    puts("클라이언트 소켓 연결 에러(1)!");
    exit(1);
  } else {
    save_log("새로운 연결 생성", msg, sock_fd, argv[2]);
  }

  // 서버에게 데이터 요청 및 응답처리
  while(1) {
    fgets(msg.message, MASSAGE_SIZE, stdin);
    msg.message[strlen(msg.message) -1] = '\0'; // fgets는 문자열의 맨 끝에 개행 문자를 삽입하게 되므로 개행문자를 지워줘야한다. 여기서는 문자열의 맨 끝임을 알리는 '\0'으로 대신해주었다.
    
    save_log(msg.message, msg, sock_fd, argv[2]);
    
    if (!strncmp(msg.message, "exit", 4)) {
      read(sock_fd, exit_msg, 4);
      if (!strncmp(exit_msg, "exit", 4)) {
        save_log("클라이언트 연결 종료", msg, sock_fd, argv[2]);
        break;
      }
    }
  }

  // 프로그램 종료
  close(sock_fd);
  exit(0);
}

