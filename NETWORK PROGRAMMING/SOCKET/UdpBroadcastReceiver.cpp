//udp 브로드캐스트 수신
#include "WinSocketVSCode.h"

#define LOCAL_PORT 9000
#define BUF_SIZE    512

int main(int argc, char* argv[]) {
	int retval;

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	//UDP용 소켓 생성
	SOCKET udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock == INVALID_SOCKET) err_quit("socket()");

	//bind()
	struct sockaddr_in localaddr;
	memset(&localaddr, 0, sizeof(localaddr));
	localaddr.sin_family = AF_INET;//AF_INET은 ipv4
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY은 0으로 모든 클라이언트를 받음
	localaddr.sin_port = htons(LOCAL_PORT);
	retval = bind(udp_sock, (struct sockaddr*)&localaddr, sizeof(localaddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//데이터 통신에 사용할 변수
	struct sockaddr_in peeraddr;
	int addrlen;
	char buf[BUF_SIZE + 1];

	//브로드캐스트 데이터 통신
	while (1) {
		//데이터 받기
		addrlen = sizeof(peeraddr);
		retval = recvfrom(udp_sock, buf, BUF_SIZE, 0, (struct sockaddr *)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			err_print_msg("recvfrom()");
			break;
		}

		//받은 데이터 출력
		char addr[INET_ADDRSTRLEN];
		buf[retval] = '\0';
		inet_ntop(AF_INET, &peeraddr.sin_addr, addr, sizeof(addr));
		printf("[UDP|%s:%d] %s\n", addr, ntohs(peeraddr.sin_port), buf);
	}
	//소켓 닫기
	closesocket(udp_sock);

	//윈속 종료
	WSACleanup();
	return 0;
}