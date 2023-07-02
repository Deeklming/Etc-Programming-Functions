#include "WinSocketVSCode.h"

void GetHostInfo4Domain(const char* name);
void GetHostInfo4IP(struct in_addr addr);

#define DNS_NAME "www.google.com"
#define DNS_NAME2 "www.naver.com"

int main(){
    // 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;
	printf("WinSock Init success\n");

	// 소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("socket create success\n");

	// 소켓 닫기
	closesocket(sock);

	// 바이트 정렬
	u_short x1 = 0x1234, x2;
	u_long y1 = 0x12345678, y2;

	printf("\n[host byte order -> network byte order]\n");
	printf("%#x -> %#x\n", x1, x2 = htons(x1));
	printf("%#lx -> %#lx\n", y1, y2 = htonl(y1));

	printf("[network byte order -> host byte order]\n");
	printf("%#x -> %#x\n", x2, ntohs(x2));
	printf("%#lx -> %#lx\n", y2, ntohl(y2));

	printf("[byte order wrong use]\n");
	printf("%#x -> %#lx\n", x1, htonl(x1));//크기가 다름

	// IPv4 주소 변환
	const char *ipv4T = "123.45.67.89";
	printf("\nipv4(original): %s\n", ipv4T);

	// inet_pton
	struct in_addr ipv4num;
	inet_pton(AF_INET, ipv4T, &ipv4num);
	printf("ipv4(pton after): %#lx\n", ipv4num.s_addr);

	// inet_addr, 옛날 함수
	struct in_addr test4;
	test4.s_addr = inet_addr(ipv4T);
	printf("test4(inet_addr): %#lx\n", test4.s_addr);

	// WSAStringToAddressA, windows 전용
	const char *ipv4Twtest = "123.45.67.89:9000";
	SOCKADDR_IN wtest4addr;
	int wtest4size = sizeof(wtest4addr);
	WSAStringToAddressA((LPSTR)ipv4Twtest, AF_INET, NULL, (SOCKADDR*)&wtest4addr, &wtest4size);
	printf("WSAStringToAddressA: %#lx, %#hx\n", wtest4addr.sin_addr.s_addr, wtest4addr.sin_port);

	// inet_ntop
	char ipv4str[INET_ADDRSTRLEN];//INET_ADDRSTRLEN: ipv4 최대 크기
	inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
	printf("ipv4(ntop reafter): %s\n", ipv4str);

	// inet_ntoa, 옛날 함수
	char *test4str;
	test4str = inet_ntoa(test4);
	printf("test4(inet_ntoa): %s\n", test4str);

	// WSAAddressToStringA, windows 전용
	char wtest4str[INET_ADDRSTRLEN+6];//6은 :+포트 크기
	wtest4size = sizeof(wtest4str);
	WSAAddressToStringA((SOCKADDR*)&wtest4addr, sizeof(wtest4addr), NULL, wtest4str, (LPDWORD)&wtest4size);
	printf("WSAAddressToStringA: %s\n", wtest4str);

	// IPv6 주소 변환
	const char *ipv6T = "2002:1231:abcd:effa:1234:ab00:fffb:1112";
	printf("\nipv6(original): %s\n", ipv6T);

	// inet_pton
	struct in6_addr ipv6num;
	inet_pton(AF_INET6, ipv6T, &ipv6num);
	printf("ipv6(pton after): 0x");
	for(int i=0; i<16; i++) printf("%02x", ipv6num.s6_addr[i]);

	// inet_ntop
	char ipv6str[INET6_ADDRSTRLEN];//INET6_ADDRSTRLEN: ipv6 최대 크기
	inet_ntop(AF_INET6, &ipv6num, ipv6str, sizeof(ipv6str));
	printf("\nipv6(ntop reafter): %s\n", ipv6str);

	// DNS
	printf("\nDomain Name(before): %s\n", DNS_NAME);
	
	// 도메인 이름을 IP주소로
	struct in_addr addr;
	if(GetIPAddr(DNS_NAME, &addr)){
		char dnsstr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr, dnsstr, sizeof(dnsstr));
		printf("IP addr(after): %s\n", dnsstr);

		//IP주소를 도메인 이름으로
		char dnsname[256];
		if(GetDomainName(addr, dnsname, sizeof(dnsname))){
			printf("Domain Name(reafter): %s\n", dnsname);//거의 별명이 나옴
		}
	}

	// 도메인 정보 출력
	GetHostInfo4Domain(DNS_NAME2);
	GetHostInfo4IP(addr);

	// 윈속 종료
	WSACleanup();
    return 0;
}

// 도메인으로 호스트의 모든 별명과 ipv4 우선 주소 출력
void GetHostInfo4Domain(const char* name) {
	struct hostent* ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_print_msg("GetHostInfo4Domain()");
		return;
	}

	printf("\nhost official domain: %s\n", ptr->h_name);
	for(int i=0; ptr->h_aliases[i]; i++){
		printf("host aliases[%d]: %s\n", i+1, ptr->h_aliases[i]);
	}

	printf("host ip type: %s\n",
		(ptr->h_addrtype==AF_INET)?"AF_INET":
		(ptr->h_addrtype==AF_INET6)?"AF_INET6":"AF_BTH");

	for(int i=0; ptr->h_addr_list[i]; i++){
		printf("host ip[%d]: %s\n", i+1, inet_ntoa(*(struct in_addr*)ptr->h_addr_list[i]));
	}
}

// IP주소로 호스트의 모든 별명과 ipv4 주소 출력
void GetHostInfo4IP(struct in_addr addr) {
	struct hostent* ptr = gethostbyaddr((const char*)&addr, sizeof(addr), AF_INET);
	if (ptr == NULL) {
		err_print_msg("GetHostInfo4IP() null");
		return;
	}
	if (ptr->h_addrtype != AF_INET) {
		err_print_msg("GetHostInfo4IP() addrtype");
		return;
	}

	printf("\nhost official domain: %s\n", ptr->h_name);
	for(int i=0; ptr->h_aliases[i]; i++){
		printf("host aliases[%d]: %s\n", i+1, ptr->h_aliases[i]);
	}

	printf("host ip type: AF_INET\n");

	for(int i=0; ptr->h_addr_list[i]; i++){
		printf("host ip[%d]: %s\n", i+1, inet_ntoa(*(struct in_addr*)ptr->h_addr_list[i]));
	}
}