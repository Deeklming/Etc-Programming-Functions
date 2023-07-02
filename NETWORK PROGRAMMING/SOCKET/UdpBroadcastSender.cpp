//udp 브로드캐스트 송신
#include "WinSocketVSCode.h"

#define REMOTE_IP "255.255.255.255"
#define REMOTE_PORT 9000
#define BUF_SIZE    512

int main(int argc, char* argv[]) {
	int retval;

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	//UDP용 소켓 생성
	SOCKET udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock == INVALID_SOCKET) err_quit("socket()");

	//브로드캐스팅 활성화
	DWORD bEnable = 1;
	retval = setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, (const char *)&bEnable, sizeof(bEnable));
	if(retval == SOCKET_ERROR) err_quit("setsockopt()");

	//소켓 주소 구조체 초기화
	struct sockaddr_in remoteaddr;
	memset(&remoteaddr, 0, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;//AF_INET은 ipv4
	inet_pton(AF_INET, REMOTE_IP, &remoteaddr.sin_addr);
	remoteaddr.sin_port = htons(REMOTE_PORT);

	//데이터 통신에 사용할 변수
	char buf[BUF_SIZE + 1];
	int len;

	//브로드캐스트 데이터 통신
	while (1) {
		//데이터 입력
		printf("\nsend data: ");
		if(fgets(buf, BUF_SIZE+1, stdin) == NULL) break;

		// '\n' 제거
		len = (int)strlen(buf);
		if(buf[len-1] == '\n') buf[len-1] = '\0';
		if(strlen(buf) == 0) break;

		//데이터 보내기
		retval = sendto(udp_sock, buf, (int)strlen(buf), 0, (struct sockaddr *)&remoteaddr, sizeof(remoteaddr));
		if (retval == SOCKET_ERROR) {
			err_print_msg("sendto()");
			break;
		}
		printf("UDP Client: send %dbytes\n", retval);
	}
	//소켓 닫기
	closesocket(udp_sock);

	//윈속 종료
	WSACleanup();
	return 0;
}