/*
===========================================================================
Name        : SHOW_LOG_DATA
Author      : Park Woo Rim
Version     : v1.0
Copyright   : Park Woo Rim
Description : 로그 파일의 데이터를 최신 것 부터 보여준다.

구분    날짜        내용
----   ---------- -----------------------------------------------------------
생성    2020/05/18 로그 출력 프로그램 생성 : main()
삭제    2020/05/25 불필요한 기능(재조회, 앞으로, 뒤로, 환경변수 조회) 삭제 : print_log()
수정    2020/05/25 get_log_file() => set_logfile() 함수명 수정 : set_logfile()
수정    2020/05/25 로그를 최근 10개의 데이터만 출력하도록 수정 : print_log()
추가    2020/05/25 부모 프로세스가 종료될 수 있도록 ALARM 신호를 전달해주는 자식 프로세스 추가 : print_log()
추가    2020/05/25 SIGALRM 신호 발생시 수행할 __exit__ 함수 추가 : print_log()

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
int main() {
  
  // 변수 선언
  char log_file[ENV_VALUE_SIZE], log_path[ENV_VALUE_SIZE]; // 로그가 기록되어 있는 파일(writelog.txt)의 이름이 저장될 변수

  set_logfile(log_file, log_path);  // 로그파일 세팅
  print_log(log_path);  // 로그 출력

  // 프로그램 종료
  exit(0);
}

