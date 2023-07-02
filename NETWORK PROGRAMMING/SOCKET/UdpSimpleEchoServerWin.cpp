//에코 udp ipv4 서버
#include "WinSocketVSCode.h"

#define SERVER_PORT 9000
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
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;//AF_INET은 ipv4
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY은 0으로 모든 클라이언트를 받음
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = bind(udp_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//데이터 통신에 사용할 변수
	struct sockaddr_in clientaddr;
	int addrlen;
	char buf[BUF_SIZE + 1];

	//클라이언트와 데이터 통신
	while (1) {
		//데이터 받기
		addrlen = sizeof(clientaddr);
		retval = recvfrom(udp_sock, buf, BUF_SIZE, 0, (struct sockaddr *)&clientaddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			err_print_msg("recvfrom()");
			break;
		}

		//받은 데이터 출력
		char addr[INET_ADDRSTRLEN];
		buf[retval] = '\0';
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("[UDP|%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

		//데이터 보내기
		retval = sendto(udp_sock, buf, retval, 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
		if (retval == SOCKET_ERROR) {
			err_print_msg("sendto()");
			break;
		}
	}
	//소켓 닫기
	closesocket(udp_sock);

	//윈속 종료
	WSACleanup();
	return 0;
}