#include <sys/types.h> // 기본 타입 정의
#include <sys/socket.h> // socket(), AF_INET 등
#include <arpa/inet.h> // htons(), htonl() 등
#include <netdb.h> // gethostbyname() 등
#include <unistd.h> // close() 등
#include <fcntl.h> // fcntl() 등
#include <pthread.h> // pthread_create() 등
#include <poll.h> // poll()
#include <sys/epoll.h> // epoll()

#include <stdio.h> // printf() 등
#include <stdlib.h> // exit() 등
#include <string.h> // strerror() 등
#include <errno.h> // errno

// Windows 소켓 코드와 호환성을 위한 정의
typedef int SOCKET;
#define SOCKET_ERROR   -1
#define INVALID_SOCKET -1

// 소켓 함수 오류 출력 후 종료
void err_quit(const char *msg)
{
	char *msgbuf = strerror(errno);
	printf("[%s] %s\n", msg, msgbuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_print_msg(const char *msg)
{
	char *msgbuf = strerror(errno);
	printf("[%s] %s\n", msg, msgbuf);
}

// 소켓 함수 오류 출력
void err_print_code(int errcode)
{
    char *msgbuf = strerror(errcode);
    printf("[오류] %s\n", msgbuf);
}

// 도메인 이름 -> ipv4 주소
bool GetIPAddr(const char* name, struct in_addr* addr) {
	struct hostent* ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_print_msg("gethostbyname()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET) return false;
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return true;
}

// ipv4 주소 -> 도메인 이름
bool GetDomainName(struct in_addr addr, char* name, int namelen) {
	struct hostent* ptr = gethostbyaddr((const char*)&addr, sizeof(addr), AF_INET);
	if (ptr == NULL) {
		err_print_msg("gethostbyaddr()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET) return false;
	strncpy(name, ptr->h_name, namelen);
	return true;
}