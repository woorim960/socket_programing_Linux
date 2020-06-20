// 헤더 선언
#include "../headers/init.h"
#include "../headers/structures.h"
#include "../headers/functions.h"
#include <time.h>

// 로그 저장 함수 => 매개변수(저장할 문자열, RECEIVE_RECORD 구조체, 데이터를 write할 파일 디스크립터, 포트번호(문자열))
void save_log(char* data, RECEIVE_RECORD msg, int fd, char* port) {

  // "%s %s %s" 형식에 맞춰 메세지가 작성된다.
  sprintf(
    msg.message,
    "%s %s %s",
    data, get_ip(), port
  );

  // 날짜 데이터를 다루기 위한 변수 선언
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  // 현재 날짜와 시간을 msg.datetime에 저장(날짜와 시간이 정수형이기 때문에 문자열로 변환한 뒤 저장한다).
  sprintf(
    msg.datetime, 
    "%04d/%02d/%02d %02d:%02d:%02d", 
    tm.tm_year+1900, 
    tm.tm_mon+1, 
    tm.tm_mday, 
    tm.tm_hour, 
    tm.tm_min, 
    tm.tm_sec
  );
  
  // 저장될 로그 형식에 맞춰 변환된 메세지를 파일(fd)에 저장
  if (write(fd, &msg, sizeof(msg)) == -1) {
    puts("save_log() 쓰기 에러");
    exit(0);
  }
}