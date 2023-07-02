//에코 udp ipv6 서버
#include "UnixSocket.h"

#define SERVER_PORT 9000
#define BUF_SIZE    512

int main(int argc, char* argv[]) {
	int retval;

	//UDP용 소켓 생성
	SOCKET udp_sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (udp_sock == INVALID_SOCKET) err_quit("socket()");

	//bind()
	struct sockaddr_in6 serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;//AF_INET6은 ipv6
	serveraddr.sin6_addr = in6addr_any;//in6addr_any는 모든 클라이언트를 받음
	serveraddr.sin6_port = htons(SERVER_PORT);
	retval = bind(udp_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//데이터 통신에 사용할 변수
	struct sockaddr_in6 clientaddr;
	socklen_t addrlen;
	char buf[BUF_SIZE + 1];

	//클라이언트와 데이터 통신
	while (1) {
		//데이터 받기
		addrlen = sizeof(clientaddr);
		retval = recvfrom(udp_sock, buf, BUF_SIZE, 0, (struct sockaddr*)&clientaddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			err_print_msg("recvfrom()");
			break;
		}

		//받은 데이터 출력
		char addr[INET6_ADDRSTRLEN];
		buf[retval] = '\0';
		inet_ntop(AF_INET6, &clientaddr.sin6_addr, addr, sizeof(addr));
		printf("[UDP|%s,%d] %s\n", addr, ntohs(clientaddr.sin6_port), buf);

		//데이터 보내기
		retval = sendto(udp_sock, buf, retval, 0, (struct sockaddr*)&clientaddr, sizeof(clientaddr));
		if (retval == SOCKET_ERROR) {
			err_print_msg("sendto()");
			break;
		}
	}
	//소켓 닫기
	close(udp_sock);
	return 0;
}