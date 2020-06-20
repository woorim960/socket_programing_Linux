#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

// 현재 호스트의 아이피를 반환해주는 함수
char* get_ip() {

  char host_name[80];
  struct hostent *host_entry;
  int ndx;

  if ( 0 != gethostname( host_name, sizeof( host_name)))
  {
    printf( "gethostname() 실행 실패/n");
    exit( 1);
  }       
  

  host_entry = gethostbyname( host_name);

  if ( !host_entry)
  {
    printf( "gethostbyname() 실행 실패/n");
    exit( 1);
  }

  // 주석처리 된 부분은 랜카드에 따라 네트워크 주소가 여러개 일 수 있으므로 모두 출력하기 위한 코드다.
  // for ( ndx = 0; NULL != host_entry->h_addr_list[ndx]; ndx++)
  //     printf( "%s\n", inet_ntoa( *(struct in_addr*)host_entry->h_addr_list[ndx]));

  // 아이피가 있으면 반환해줌, 아이피가 여러개일 경우 첫 번째의 아이피만 반환해주도록 하였음.
  if (host_entry->h_addr_list[0]) {
    return inet_ntoa( *(struct in_addr*)host_entry->h_addr_list[0]);
  }

  return "0";
}