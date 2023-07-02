//에코 tcp ipv4 서버
#include "UnixSocket.h"

#define SERVER_PORT 9000
#define BUF_SIZE    512

int main(int argc, char* argv[]) {
	int retval;

	//리스닝용 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	//bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;//AF_INET은 ipv4
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY은 0으로 모든 클라이언트를 받음
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);//SOMAXCONN은 최대 길이
	if (retval == SOCKET_ERROR) err_quit("listen()");

	//데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	char buf[BUF_SIZE + 1];

	while (1) {
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_print_msg("accept()");
			break;
		}

		//접속한 클라이언트 정보
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("TCP accept client: ip=%s, port=%d\n", addr, ntohs(clientaddr.sin_port));

		//클라이언트와 데이터 통신
		while (1) {
			//데이터 받기
			retval = recv(client_sock, buf, BUF_SIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_print_msg("recv()");
				break;
			}
			else if (retval == 0) break;

			//받은 데이터 출력
			buf[retval] = '\0';
			printf("[TCP|%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);

			//데이터 보내기
			retval = send(client_sock, buf, retval, 0);
			if (retval == SOCKET_ERROR) {
				err_print_msg("send()");
				break;
			}
		}

		//소켓 닫기
		close(client_sock);
		printf("TCP close client: ip=%s, port=%d\n", addr, ntohs(clientaddr.sin_port));
	}

	//소켓 닫기
	close(listen_sock);

	return 0;
}