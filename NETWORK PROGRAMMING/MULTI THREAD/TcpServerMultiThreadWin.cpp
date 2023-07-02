//멀티스레드 에코 tcp ipv4 서버
#include "WinSocketVSCode.h"

#define SERVER_PORT 9000
#define BUF_SIZE    512

DWORD WINAPI ProcessClient(LPVOID arg){//클라이언트와 통신용, echo
	//데이터 통신에 사용할 변수
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	int retval, addrlen;
	char addr[INET_ADDRSTRLEN], buf[BUF_SIZE + 1];

	//접속한 클라이언트 정보
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);//클라이언트의 ip와 port정보를 얻음
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

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
	closesocket(client_sock);
	printf("TCP close client: ip=%s, port=%d\n", addr, ntohs(clientaddr.sin_port));
	return 0;
}

int main(int argc, char* argv[]) {
	int retval;

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

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
	HANDLE hThread;//스레드용

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

		//스레드로 처리
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if(hThread==NULL) closesocket(client_sock);
		else CloseHandle(hThread);
	}

	//소켓 닫기
	closesocket(listen_sock);

	//윈속 종료
	WSACleanup();
	return 0;
}