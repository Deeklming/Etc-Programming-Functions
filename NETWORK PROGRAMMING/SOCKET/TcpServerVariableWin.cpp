//데이터 가변 길이 전송 tcp ipv4 서버
#include "WinSocketVSCode.h"

#define SERVER_PORT 9000
#define BUF_SIZE    512//받은 데이터 길이가 이 길이보다 작으면 가변 길이 데이터로 간주함

//소켓 수신 버퍼에서 데이터를 한꺼번에 읽어 내부에 저장하고 읽기 요청이 있을 시 1byte씩 리턴함
int _recv_ahead(SOCKET s, char *p){
	__declspec(thread) static int nbytes = 0;//__declspec(thread)는 멀티스레드 환경에서 필요
	__declspec(thread) static char buf[1024];//함수가 리턴해도 값 유지를 위해 static 선언
	__declspec(thread) static char *ptr;//linux에서는 static __thread char *ptr; 방식으로 사용
	
	if(nbytes==0 || nbytes==SOCKET_ERROR){
		nbytes = recv(s, buf, sizeof(buf), 0);//buf에 데이터 저장해 둠
		if(nbytes==SOCKET_ERROR){
			return SOCKET_ERROR;
		}
		else if(nbytes==0) return 0;
		ptr = buf;//ptr은 buf의 맨 앞을 가리킴
	}

	--nbytes;//남은 byte 1 감소
	*p = *ptr++;//데이터를 1byte 복사
	return 1;
}

//사용자가 정의한 데이터 수신 함수
int recvline(SOCKET s, char *buf, int maxlen){
	int n, nbytes;
	char c, *ptr = buf;

	for(n=1; n<maxlen; n++){//maxlen-1까지
		nbytes = _recv_ahead(s, &c);//1byte씩 읽음
		if(nbytes==1){
			*ptr++ = c;
			if(c=='\n') break;//\n이 나올 때까지
		}
		else if(nbytes==0){
			*ptr = 0;
			return n-1;
		}
		else return SOCKET_ERROR;
	}

	*ptr = 0;
	return n;
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
			retval = recvline(client_sock, buf, BUF_SIZE+1);
			if (retval == SOCKET_ERROR) {
				err_print_msg("recv()");
				break;
			}
			else if (retval == 0) break;

			//받은 데이터 출력
			printf("[TCP|%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);
		}

		//소켓 닫기
		closesocket(client_sock);
		printf("TCP close client: ip=%s, port=%d\n", addr, ntohs(clientaddr.sin_port));
	}

	//소켓 닫기
	closesocket(listen_sock);

	//윈속 종료
	WSACleanup();
	return 0;
}