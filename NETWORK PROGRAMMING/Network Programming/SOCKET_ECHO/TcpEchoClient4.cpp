//에코 tcp ipv4 클라이언트
#include "Common.h"

char *SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512

int main(int argc, char* argv[]) {
	int retval;

    //명령행 인수가 있다면
    if(argc>1) SERVERIP = argv[1];

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	//소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

    //connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	//데이터 통신에 사용할 변수
	int len;
	char buf[BUFSIZE + 1];

    //서버와 통신
	while (1) {
		//데이터 입력
        printf("\ndata: ");
        if(fgets(buf, BUFSIZE+1, stdin)==NULL) break;
        
        //\n제거
        len = (int)strlen(buf);
        if(buf[len-1]=='\n') buf[len-1]='\0';
        if(strlen(buf)==0) break;//그냥 enter입력 시 종료

        //데이터 보내기
        retval = send(sock, buf, (int)strlen(buf), 0);
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
        printf("TCP client: send %dbyte.\n", retval);

        //데이터 받기
        retval = recv(sock, buf, retval, MSG_WAITALL);//MSG_WAITALL은 len 크기만큼 데이터가 수신버퍼에 도착해서 응용 프로그램의 저장 버퍼에 전부 복사할 때까지 기다림
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0) break;

        //받은 데이터 출력
        buf[retval] = '\0';
        printf("[TCP client] recv %dbyte.\n", retval);
        printf("recv data: %s\n", buf);
	}

	//소켓 닫기
	closesocket(sock);

	//윈속 종료
	WSACleanup();
	return 0;
}