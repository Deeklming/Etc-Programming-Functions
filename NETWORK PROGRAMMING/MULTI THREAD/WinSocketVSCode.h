#pragma once

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더, sockaddr_in, byte order function
#include <ws2tcpip.h> // 윈속2 확장 헤더, sockaddr_in6, ip주소 변환 함수

#include <tchar.h> // _T()
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <string.h> // strncpy()
#include <errno.h> // errno

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

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
