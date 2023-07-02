#pragma comment(lib, "Ws2_32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
void err(char* msg);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	char message[30];
	int strLen;
	
    if(argc!=3){
        printf("Usage: %s <ip> <port>\n", argv[0]);
		exit(1);
    }

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) err("WSAStartup err");
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) err("socket err");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	//servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	inet_pton(AF_INET, argv[1], &(servAddr.sin_addr));
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) err("bind err");
	
	strLen = recv(hSocket, message, sizeof(message) - 1, 0);
	if (strLen == -1) err("read err");
	printf("Message from server: %s\n", message);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void err(char* msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}