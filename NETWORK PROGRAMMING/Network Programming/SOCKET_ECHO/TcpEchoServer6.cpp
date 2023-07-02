//에코 tcp ipv6 서버
#include "Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512

int main(int argc, char* argv[]) {
	int retval;

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	//소켓 생성
	SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	//bind()
	struct sockaddr_in6 serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;
	serveraddr.sin6_addr = in6addr_any;//서버에서 in6addr_any는 모든 ipv6주소를 받아 드림
	serveraddr.sin6_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);//2번째 인자 backlog는 접속 가능한 클라이언트 수로 연결 큐에 저장되는데 SOMAXCONN는 최댓값임
	if (retval == SOCKET_ERROR) err_quit("listen()");

	//데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in6 clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1) {
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		//접속한 클라이언트 정보
		char addr[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &clientaddr.sin6_addr, addr, sizeof(addr));
		printf("TCP6 accept client: ip=%s, port=%d\n", addr, ntohs(clientaddr.sin6_port));

		//클라이언트와 데이터 통신
		while (1) {
			//데이터 받기
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0) break;

			//받은 데이터 출력
			buf[retval] = '\0';
			printf("[TCP6|%s:%d] %s\n", addr, ntohs(clientaddr.sin6_port), buf);

			//데이터 보내기
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
		}

		//소켓 닫기
		closesocket(client_sock);
		printf("TCP6 close client: ip=%s, port=%d\n", addr, ntohs(clientaddr.sin6_port));
	}

	//소켓 닫기
	closesocket(listen_sock);

	//윈속 종료
	WSACleanup();
	return 0;
}