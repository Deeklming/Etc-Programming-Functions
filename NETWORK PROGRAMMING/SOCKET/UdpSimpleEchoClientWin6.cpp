//에코 udp ipv6 클라이언트
#include "WinSocketVSCode.h"

char *SERVER_IP = (char*)"::1";
#define SERVER_PORT 9000
#define BUF_SIZE    512

int main(int argc, char* argv[]) {
	int retval;

	//명령행 인수가 있다면
    if(argc>1) SERVER_IP = argv[1];

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	//UDP용 소켓 생성
	SOCKET udp_sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (udp_sock == INVALID_SOCKET) err_quit("socket()");

	//소켓 주소 구조체 초기화
	struct sockaddr_in6 serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET6;//AF_INET6은 ipv6
	inet_pton(AF_INET6, SERVER_IP, &serveraddr.sin6_addr);
	serveraddr.sin6_port = htons(SERVER_PORT);

	//데이터 통신에 사용할 변수
	struct sockaddr_in6 peeraddr;
	int addrlen;
	char buf[BUF_SIZE + 1];
	int len;

	//서버와 데이터 통신
	while (1) {
		//데이터 입력
		printf("\nsend data: ");
		if(fgets(buf, BUF_SIZE+1, stdin) == NULL) break;

		// '\n' 제거
		len = (int)strlen(buf);
		if(buf[len-1] == '\n') buf[len-1] = '\0';
		if(strlen(buf) == 0) break;

		//데이터 보내기
		retval = sendto(udp_sock, buf, (int)strlen(buf), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) {
			err_print_msg("sendto()");
			break;
		}
		printf("UDP Client: send %dbytes.\n", retval);

		//데이터 받기
		addrlen = sizeof(peeraddr);
		retval = recvfrom(udp_sock, buf, BUF_SIZE, 0, (struct sockaddr*)&peeraddr, &addrlen);
		if(retval == SOCKET_ERROR){
			err_print_msg("recvfrom()");
			break;
		}

		//송신자 IP 주소 체크
		if(memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))){
			printf("error: wrong data!\n");
			break;
		}

		//받은 데이터 출력
		buf[retval] = '\0';
		printf("UDP Client: get %dbytes from server\n", retval);
		printf("data: %s\n", buf);
	}
	//소켓 닫기
	closesocket(udp_sock);

	//윈속 종료
	WSACleanup();
	return 0;
}