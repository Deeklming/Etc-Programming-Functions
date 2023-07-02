#pragma once

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더, sockaddr_in, byte order function
#include <ws2tcpip.h> // 윈속2 확장 헤더, sockaddr_in6, ip주소 변환 함수

#include <tchar.h> // _T()
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <string.h> // strncpy()

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

// 소켓 함수 오류 출력 후 종료
//리눅스와 통일을 위해 ANSI 버전 사용 만약 유니코드 버전을 사용하고 싶다면
//void err_quit(const wchar_t* msg)로 변경하고 호출 시 err_quit(L"message")로 사용
//그리고 FormatMessageW와 MessageBoxW로 변경해서 사용
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(//ANSI 버전
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);//ANSI 버전
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display_msg(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력
void err_display_code(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 도메인 이름 -> ipv4 주소
bool GetIPAddr(const char* name, struct in_addr* addr) {
	struct hostent* ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display_msg("gethostbyname()");
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
		err_display_msg("gethostbyaddr()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET) return false;
	strncpy(name, ptr->h_name, namelen);
	return true;
}
