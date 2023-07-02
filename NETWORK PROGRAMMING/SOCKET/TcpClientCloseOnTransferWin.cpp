//데이터 전송 후 종료 방식 tcp ipv4 클라이언트
#include "WinSocketVSCode.h"

char *SERVER_IP = (char*)"127.0.0.1";
#define SERVER_PORT 9000
#define BUF_SIZE    50//보낼 데이터 크기

int main(int argc, char* argv[]) {
	int retval;

    //명령행 인수가 있다면
    if(argc>1) SERVER_IP = argv[1];

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    //connect() 호출용 변수
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;//AF_INET은 ipv4
    inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVER_PORT);

	//데이터 통신에 사용할 변수
	char buf[BUF_SIZE];
    const char *testdata[] = {//랜덤한 길이의 테스트용 데이터
        "이런이런",
        "This is first",
        "It is test data ok? okok. yes.",
        "check please!!",
    };
	int len;//보낼 문자열 길이 저장용

    //서버와 통신, 소켓의 생성, 접속, 닫기를 반복
	for(int i=0; i<4; i++) {
		//소켓 생성
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) err_quit("socket()");

		//connect()
		retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) err_quit("connect()");

		//데이터 입력
        len = (int)strlen(testdata[i]);//가변 길이
        strncpy(buf, testdata[i], len);//문자열 데이터 버퍼 복사
        
        //데이터 보내기
        retval = send(sock, buf, len, 0);//len 크기 만큼 보냄
        if (retval == SOCKET_ERROR) {
            err_print_msg("send()");
            break;
        }
        printf("TCP client: send %dbyte.\n", retval);

		//소켓 닫기
		closesocket(sock);
	}

	//윈속 종료
	WSACleanup();
	return 0;
}