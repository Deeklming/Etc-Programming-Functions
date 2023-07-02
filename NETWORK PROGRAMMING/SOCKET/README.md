# SOCKET
Socket Programming

## Socket Features
1. 저수준 프로그래밍 인터페이스로 세부 제어가 가능하고 고성능 네트워크 프로그램을 개발 할 수 있음
2. 프로그래머가 응용 프로그램 수준의 프로토콜을 직접 설계
3. 서로 다른 데이터 처리 단위(32bit, 64bit)와 바이트 순서(Byte Ordering)를 사용하는 호스트끼리 통신할 경우 응용 프로그램 수준에서 데이터 변환을 처리해야 함

## Socket theory
### Socket Header
에러처리 등 전역에 필요한 것 및 정의
#### UnixSocket.h
유닉스와 리눅스용 헤더
#### WinSocketVS.h
윈도우에서 Visual Studio를 사용할 때 쓰는 헤더
#### WinSocketVSCode.h
윈도우에서 Visual Studio Code를 사용할 때 쓰는 헤더 (기본으로 사용)
### Socket Basic
socket()은 핸들값인 소켓 디스크립터(Socket Descriptor)를 반환함
```
    Address family(주소 체계)

    IPv4: AF_INET
    IPv6: AF_INET6
    Bluetooth: AF_BTH
```
```
    Socket type(소켓 타입)

    TCP: SOCK_STREAM
    UDP: SOCK_DGRAM
```
```
    Protocol(프로토콜)

    TCP: IPPROTO_TCP
    UDP: IPPROTO_UDP
    ※ 여러 프로토콜이 있지만 TCP와 UDP 프로토콜은 주소 체계와 소켓 타입만으로 결정 가능 해서 0으로 사용
```
```
    socket() 최종 설정
    
    TCP: (Address family) SOCK_STREAM 0
    UDP: (Address family) SOCK_DGRAM 0
```
```
    sockaddr 구조체

    IPv4: sockaddr_in
    IPv6: sockaddr_in6
    Bluetooth: sockaddr_bth

    sin_family, sin6_family: AF_INET, AF_INET6
    sin_port, sin6_port: 부호 없는 16비트 정숫값 포트 번호
    sin_addr, sin6_addr: 32bit in_addr 구조체, 128bit in6_addr 구조체인 IP주소

    ※ 소켓 주소 구조체는 커서 항상 (sockaddr*)로 형변환 해야 하고 프로토콜에 따라 크기가 달라 sizeof 연산자로 크기를 같이 인수로 전달해야 함
```
```
    Byte Order(바이트 정렬(순서)) - CPU와 OS에 따라 다름
    
    Big-endian: MSB(Most Significant Byte) 최상위 바이트부터 차례로 저장
    Little-endian: LSB(Least Significant Byte) 최하위 바이트부터 차례로 저장
    Network Byte Order(네트워크 바이트 정렬): IP주소와 Port번호를 Big-endian으로 통일 함
    ※ 서버와 클라이언트도 통일해서 구현, 보통 서버에 따라 맞춤

    byte order function
    htons(u_short host): host->network 16bit(s)
    htonl(u_long host): host->network 32bit(l)
    ntohs(u_short net): network->host 16bit(s)
    ntohl(u_long net): network->host 32bit(l)
```
```
    IP주소 변환

    inet_pton(): 문자열->숫자(presentation to numeric)
    inet_ntop(): 숫자->문자열(numeric to presentation)
    ※ 마지막 인수 크기 타입이 윈도우는 size_t, 리눅스는 socklen_t으로 다름
```
```
    DNS(Domain Name System) 서버
    
    Domain Name: IP주소와 마찬가지로 호스트나 라우터의 고유한 식별자
    DNS Server는 일종의 Distributed Database(분산 데이터베이스) 임
    struct hostent 구조체를 사용하여 gethostbyname, gethostbyaddr 등 함수가 작성 됨
    *gethostbyname(), *gethostbyaddr(): 도메인 이름을 IP주소(네트워크 바이트 정렬)로, IP주소(네트워크 바이트 정렬)를 도메인 이름으로
    ※ 위의 두 함수는 IPv4만 사용함, IPv4/IPv6를 사용하려면 getaddrinfo()과 freeaddrinfo()로 할당 해제, getnameinfo()를 사용해야 함
    Domain Name에서 유효한 도메인 이름을 IP주소로 변환하는 것은 가능하지만 IP주소를 도메인 이름으로 변환하는 것은 항상 가능한 것이 아님
    hostent 구조체는 스레드당 하나씩 할당해서 변환된 IP주소나 도메인 이름을 계속 사용하려면 다른 소켓 함수를 호출하기 전에 미리 복사해두어야 함
    ※ IP주소를 도메인 이름으로 변경 시 호스트가 대표 도메인 이름 외에 별명이 있으면 원래의 이름과 다른 결과가 나올 수 있음
```
#### SocketBasicLinux.cpp - linux
```
    <data type>
    int 타입: 소켓이 파일 디스크립터와 같아서 recv(), send() 대신 read(), write()로도 네트워크 통신 가능

    <character>
    - socket의 기본이고 따로 버전이 없음
```
리눅스의 소켓은 생성, 닫기만 있음
#### SocketBasicWin.cpp - windows
```
    <data type>
    SOCKET 타입: int 타입이 아닌 별도의 타입 사용

    <character>
    - DLL로 대부분의 기능이 제공되므로 DLL초기화와 종료의 윈도우 전용 함수가 필요하고 이외에도 많은 확장 함수가 있음
    - WS2_32.DLL을 사용해서 응용 프로그램 링크 시 WS2_32.LIB가 필요
    - 윈도우98 이후 윈속 2.2버전을 사용하고 이전에는 윈도우95 기준 윈속 1.1버전을 사용했었음
```
윈도우의 소켓은 초기화, 종료, 생성, 닫기가 있음   
WSAStartup() 사용 횟수마다 WSACleanup() 사용   
sockaddr 구조체는 C언어의 typedef로 대문자로 정의되어 있지만 리눅스와 호환을 위해 사용하지 않음
### Server & Client
**흐름**
||TCP SERVER|NETWORK|TCP CLIENT||
|:---|---|---|---|---:|
||socket||socket||
||bind||||
||listen||||
||accept|<ㅡㅡㅡ|connect||
|ㅡ>|ㅣ||ㅣ|<ㅡ|
|ㅣ|recv|<ㅡㅡㅡ|send|ㅣ|
|ㅣ|send|ㅡㅡㅡ>|recv|ㅣ|
|ㅡ|(if not close)||(if not close)|ㅡ|
||close||close||
#### Append Info
1. 소켓은 리스닝 소켓으로 클라이언트의 요청이 들어오면 시스템에서 새로운 소켓을 생성하여 그곳으로 통신을 함. 즉, 서버 하나에 클라이언트가 셋 이라면 소켓은 모두 네 개가 됨
2. 리스닝 소켓과 시스템이 새로 생성한 소켓은 같은 포트를 사용 함
3. 서버의 소켓과 클라이언트의 소켓은 일대일 대응이고 하나의 클라이언트에서 여러 소켓을 사용하여 다중 통신이 가능함
### TcpSimpleEchoServer(OS).cpp
단순한 에코 서버 ipv4
### TcpSimpleEchoClient(OS).cpp
단순한 에코 클라이언트 ipv4
#### bind()
소켓의 지역 IP주소와 지역 Port번호를 결정
```c
    int bind(소켓, 소켓 주소 구조체, 소켓 주소 구조체 길이);
```
소켓 - SOCKET(win), int(linux)   
소켓 주소 구조체 - sockaddr_in*(ipv4), sockaddr_in6*(ipv6), 서버의 정보   
소켓 주소 구조체 길이 - int(win), socklen_t(linux)
#### listen()
소켓의 TCP 상태를 LISTENING으로 바꿈
```c
    int listen(소켓, backlog);
```
소켓 - SOCKET(win), int(linux)   
backlog - int(접속 가능한 클라이언트 개수), 클라이언트 접속 정보가 있는 연결 큐의 길이, backlog 값을 바꾸려면 listen() 다시 호출
#### accept()
클라이언트 접속을 수용하고 그것과 통신할 수 있는 새로운 소켓을 생성하여 리턴함   
클라이언트가 접속 한 것은 연결 설정이 성공되었다는 뜻이며 그 클라이언트의 주소 정보도 얻을 수 있음
```c
    int accept(소켓, 소켓 주소 구조체, 소켓 주소 구조체 길이);
```
소켓 - SOCKET(win), int(linux)   
소켓 주소 구조체 - sockaddr_in*(ipv4), sockaddr_in6*(ipv6), 클라이언트의 정보   
소켓 주소 구조체 길이 - int*(win), socklen_t*(linux), 클라이언트의 길이   
접속한 클라이언트가 없을 경우 Wait State(대기 상태)가 되며 CPU는 0%를 사용함   
※ 클라이언트의 정보를 알 필요가 없다면 소켓 주소 구조체와 그 길이에 NULL을 전달하면 됨
#### connect()
TCP에서 서버와 논리적 연결을 설정
```c
    int connect(소켓, 소켓 주소 구조체, 소켓 주소 구조체 길이);
```
소켓 - SOCKET(win), int(linux)   
소켓 주소 구조체 - const sockaddr_in*(ipv4), const sockaddr_in6*(ipv6), 연결할 서버의 정보   
소켓 주소 구조체 길이 - int(win), socklen_t(linux)   
bint()를 호출하지 않은 상태에서 connect()를 호출하면 OS가 자동으로 지역 IP와 Port를 할당함
### Data send and recv
TCP에 기본은 send(), recv()가 있고 윈도우 전용으로 WSASend*(), WSARecv*()가 있고 리눅스 전용 파일 입출력 함수로 write(), read()도 가능함   
UDP에는 sendto(), recvfrom() 함수가 있음   
데이터 송수신에는 각각 버퍼가 있으며 수신할 때 기본적으로 데이터 경계를 구분하지 않음(10,20,30byte로 보내면 33, 27byte로 받을 수도 있음). 그래서 상호간에 약속이 필요함
#### send()
```c
    int send(소켓, 데이터를 담고 있는 버퍼, 보낼 데이터 크기, 플래그);
```
소켓 - SOCKET(win), int(linux)   
데이터를 담고 있는 버퍼 - const char*(win), const void*(linux)   
보낼 데이터 크기 - int(win), size_t(linux), byte단위   
플래그 - 옵션, int, 거의 0을 사용   
반환값 - int(win), ssize_t(linux)   
※ Blocking socket - 기본적으로 블로킹 소켓이며 소켓 송신 버퍼의 여유 공간이 send()의 보낼 데이터 크기보다 작을 경우 대기 상태에 들어가고 이상일 경우 프로세스가 깨어나고 그 크기만큼 복사가 일어난 후 리턴함, send()의 리턴값은 보낼 데이터 크기와 같음   
※ Nonblocking socket - 윈도우의 ioctlsocket()과 리눅스의 fcntl()로 넌블로킹 소켓으로 바꿀 수 있음, 소켓 송신 버퍼의 여유 공간만큼 데이터를 복사한 후 리턴함, send()의 리턴값은 최소 1, 최대 보낼 데이터 크기와 같음
#### recv()
```c
    int recv(소켓, 받은 데이터를 담을 버퍼, 받을 데이터 최대 크기, 플래그);
```
소켓 - SOCKET(win), int(linux)   
받은 데이터를 담을 버퍼 - char*(win), void*(linux)   
받을 데이터 최대 크기 - int(win), size_t(linux), byte단위, 받은 데이터를 담을 버퍼를 넘지 말아야 함   
플래그 - 옵션, int, 주로 MSG_PEEK와 MSG_WAITALL을 사용   
반환값 - int(win), ssize_t(linux), 수신 버퍼에서 복사한 bytes 수를 리턴함   
※ MSG_PEEK - 복사 후에도 수신 버퍼에 데이터가 계속 남음   
※ MSG_WAITALL - 수신 버퍼에 데이터를 가득 채운 후 복사하고 마지막은 기다리지 않고 바로 복사함   
※ close()나 closesocket()으로 정상적인
### TcpSimpleEchoServer(OS)6.cpp
단순한 에코 서버 ipv6
### TcpSimpleEchoClient(OS)6.cpp
단순한 에코 클라이언트 ipv6

* * *
## AP
Application Protocol(응용 프로그램 프로토콜)은 응용 프로그램 수준에서 주고받는 데이터 형식과 의미, 처리 방식을 정의한 프로토콜로 응용 프로그램마다 자유롭게 정의한다는 것이 특징이임   
### send data precondition
응용 프로그램 데이터 전송 조건
#### 조건1 - 경계 구분
TCP처럼 경계를 구분하지 않는 프로토콜에서는 데이터의 경계를 따로 구분해줘야 함   
1. 송수신자는 고정길이의 데이터를 주고 받는 방식
2. 마지막에 EOR(End Of Record)를 사용하여 데이터의 끝을 아는 방식
3. 처음에 고정길이로 헤더를 만들어 전체 길이를 미리 알려주고 가변길이로 그것만큼만 데이터를 주고 받는 방식
4. 송신자가 가변 길이 데이터로 전송한 후 연결을 정상 종료하면 수신자도 자동 종료
#### 조건2 - 구조체 멤버 맞춤
구조체나 클래스 크기만큼 보내는데 C/C++ 컴파일러에서는 메모리를 약간 낭비하는 방식을 취하기 때문에 패킹하는 옵션을 적용해주면 메모리 낭비를 없앨 수 있음   
아래 MySturct 구조체에서 기본으로 10bytes초과를 사용하지만 1bytes 경계로 패킹을 하면 10bytes만 사용함
##### linux
```c
struct __attribute__((packed)) MySturct{//__attribute__((packed))으로 패킹함
    int a; //4bytes
    char b; //1bytes
    int c; //4bytes
    char d; //1bytes
};
```
##### windows
```c
#pragma pack(1)//1bytes 경계로 패킹함
struct MySturct{
    int a; //4bytes
    char b; //1bytes
    int c; //4bytes
    char d; //1bytes
};
#pragma pack()//구조체 멤버 맞춤(패킹)을 컴파일러 기본값으로 되돌림
```
#### 조건3 - 바이트 정렬
특별한 전제가 없다면 네트워크 바이트 정렬인 빅 엔디언 방식으로 통일하면 좋음
### send data way
응용 프로그램 데이터 전송 방식
#### 고정 길이 데이터 전송
서버와 클라이언트가 같은 버퍼의 크기를 정해두고 데이터를 주고 받음
##### TcpServerFixedWin.cpp
고정 길이 전송 서버
##### TcpClientFixedWin.cpp
고정 길이 전송 클라이언트
#### 가변 길이 데이터 전송
서버와 클라이언트가 같이 EOR로 사용할 데이터 패턴을 정해서 데이터를 주고 받음   
소켓 수신 버퍼에서 1byte씩 읽으면 속도가 늦어지기 때문에 수신버퍼에서 데이터를 한꺼번에 읽어 내부에 저장해두고 읽기 요청일 있을때마다 1byte씩 리턴해주는 기능으로 작성함
##### TcpServerVariableWin.cpp
가변 길이 전송 서버 - EOR: \n로 정함
##### TcpClientVariableWin.cpp
가변 길이 전송 클라이언트 - EOR: \n로 정함
#### 고정 + 가변 길이 데이터 전송
송신 측에서 가변 길이 크기를 미리 계산할 수 있으면 이 방식이 효과적임   
두 번의 데이터 읽기 작업으로 가변 길이 데이터의 경계를 구분 할 수 있음
##### TcpServerFixedVariableWin.cpp
고정 + 가변 길이 전송 서버
##### TcpClientFixedVariableWin.cpp
고정 + 가변 길이 전송 클라이언트
#### 데이터 전송 후 종료
가변 길이 전송 방식중 하나로 EOR 패턴 대신 연결 종료를 사용하는 방식
##### TcpServerCloseOnTransferWin.cpp
데이터 전송 후 종료 방식 서버
##### TcpClientCloseOnTransferWin.cpp
데이터 전송 후 종료 방식 클라이언트

* * *
## UDP
UDP(User Datagram Protocol)는 Port 번호와 IP주소를 지정하고 데이터 오류를 체크함
### UDP 특징
1. 비연결형 프로토콜인 UDP는 연결 설정을 하지 않아서 connect()함수를 사용하지 않지만 사용 하더라도 연결을 위한 패킷 교환은 일어나지 않음
2. UDP는 신뢰성 있는 통신을 보장하지 않아서 필요하다면 응용프로그램 수준에서 기능을 구현해야 함
3. UDP는 Unicast(1:1) 또는 Broadcast or Multicast(1:다수)를 쉽게 구현할 수 있음(멀티스레드 이용하지 않아도 다수의 클라이언트 처리 가능)
4. TCP와 달리 Datagram(데이터그램)을 이용하기 때문에 데이터 경계 구분을 위한 별도의 작업을 할 필요 없음
5. UDP로 대량의 데이터를 주고받으려면 Flow Control(흐름 제어) 기능을 구현하는 것이 좋음
6. TCP와 달리 송신 버퍼가 없음
7. 블로킹 소켓을 사용할 경우 송수신 함수의 호출 순서가 맞지 않으면 Deadlock(교착 상태)가 발생함
#### UdpSimpleEchoServer(OS).cpp
단순한 에코 서버 ipv4 udp
#### UdpSimpleEchoClient(OS).cpp
단순한 에코 클라이언트 ipv4 udp   
※ UDP 클라이언트는 관련 없는 UDP 데이터를 받을 수 있기 때문에 서버로부터 데이터를 받은 후 송신자의 주소를 확인해야 함   
※ UDP 클라이언트에서 connect()를 사용할 수 있는데 고정된 서버를 정할때 사용하고 주소를 재사용하기 때문에 조금 더 효율적임, 그리고 자동으로 설정한 대상이 아닌 UDP 데이터를 수신하지 않기 때문에 따로 거를 필요가 없음, 또한 send()와 recv()함수로 대체 가능함
### UDP의 데이터 전송 함수
UDP의 데이터 전송 함수로 기본은 sendto()와 recvfrom()이고 OS에 따라 전용 함수가 있음, 윈도우는 WSASend*()와 WSARecv*()를 제공하고 리눅스는 sendmsg()와 recvmsg() 함수를 추가로 제공함   
#### sendto()
sendto() 호출 시 지역 IP와 Port가 결정되지 않은 상태이면 OS가 자동으로 결정함
```c
    int sendto(소켓, 보낼 데이터 주소, 보낼 데이터 크기, 옵션, 목적지 소켓 주소 구조체, 목적지 소켓 주소 구조체 길이);
```
소켓 - SOCKET(win), int(linux)   
보낼 데이터 주소 - const char*(win), void*(linux)   
보낼 데이터 크기 - int(win), size_t(linux), byte단위   
옵션 - int(win), int(linux)   
목적지 소켓 주소 구조체 - const struct sockaddr*(win), struct sockaddr*(linux)   
목적지 소켓 주소 구조체 길이 - int(win), socklen_t(linux), byte단위   
리턴값 - int(win), ssize_t(linux)
1. sendto()는 TCP 소켓에도 사용 가능하고 응용 프로그램 데이터는 커널영역에 복사되어 전송된 후 바로 버려짐
2. sendto()는 0 ~ 65507bytes의 데이터 크기 제한이 있고 이것보다 훨씬 작은 크기의 데이터를 전송하는 것이 좋음, UDP로 브로드캐스트 패킷을 보낸다면 512bytes보다 작은 크기가 좋음
#### recvfrom()
recvfrom()은 UDP 패킷을 한 번에 하나만 읽을 수 있음   
recvfrom() 사용 전 반드시 지역 주소 결정을 위해 bind()나 sendto()를 호출한 후 사용해야 함
```c
    int recvfrom(소켓, 받을 데이터 주소, 받을 데이터 최대 크기, 옵션, 송신자 소켓 주소 구조체, 송신자 소켓 주소 구조체 크기);
```
소켓 - SOCKET(win), int(linux)   
받을 데이터 주소 - char*(win), void*(linux)   
받을 데이터 최대 크기 - int(win), size_t(linux), byte단위   
옵션 - int(win), int(linux)   
송신자 소켓 주소 구조체 - struct sockaddr*(win), struct sockaddr*(linux)   
송신자 소켓 주소 구조체 크기 - int*(win), socklen_t*(linux), byte단위   
리턴값 - int(win), ssize_t(linux)
1. 소켓은 지역 주소 미리 결정 필수이고 받을 데이터 최대 크기를 넘는 데이터를 받으면 SOCKET_ERROR(-1)을 리턴함
2. recvfrom()은 TCP 소켓에도 사용 가능함
3. 리턴값이 0 일때 UDP에서는 특별한 의미는 없지만 TCP에서는 정상 종료를 의미함
#### UdpSimpleEchoServer(OS)6.cpp
단순한 에코 서버 ipv6 udp
#### UdpSimpleEchoClient(OS)6.cpp
단순한 에코 클라이언트 ipv6 udp
### Broadcasting
브로드캐스팅과 멀티캐스팅은 IP가 제공하는 기능이지만 응용 프로그램 수준에서는 UDP를 사용해서 구현하는 것이 일반적임
```
통신 개체 간 상호 작용
1. Unicasting(유니캐스팅): IPv4/IPv6지원, 한 개체가 다른 개체에 데이터를 보내는 모델
2. Broadcasting(브로드캐스팅): IPv4지원, 한 개체가 특정 네트워크에 속한 모든 개체에 데이터를 보내는 모델
3. Multicasting(멀티캐스팅): IPv4/IPv6지원, 한 개체가 논리적 동일 그룹에 가입한 모든 개체에 데이터를 보내는 모델
4. Anycasting(애니캐스팅): IPv6지원, 한 개체가 논리적 동일 그룹에 가입한 개체 중 하나에 보내면 그 개체가 논리적 동일 그룹에 가입한 모든 개체에 데이터를 보내는 모델
```
socket() 함수로 생성한 소켓은 기본적으로 유니캐스팅만 가능함   
|브로드캐스팅 방식||||
|---|---|---|---|
|종류|브로드캐스트 주소 구조|조건|설명|
|Net-Directed Broadcast(네트워크 브로드캐스트)|네트워크ID(16bit)호스트ID(16bit)|호스트ID가 모두 1인 경우|데이터가 라우터를 거처야 해서 거의 실용적이지 않음|
|Subnet-Directed Broadcast(서브넷 브로드캐스트)|네트워크ID(16bit)호스트ID(서브넷ID 8bit, 8bit)|서브넷ID를 제외한 호스트ID가 모두 1인 경우|외부 서브넷은 라우터를 거쳐야하므로 거의 실용적이지 않지만 송신자 내부 서브넷은 지역 브로드캐스트이므로 가능|
|Local Broadcast(지역 브로드캐스트)|전체ID(32bit)|네트워크ID와 호스트ID가 모두 1인 경우(IPv4: 255.255.255.255)|송신자 내부 네트워크이므로 항상 가능|
※ 라우터의 설정에 따라 브로드캐스팅이 불가능한 경우가 많음
#### UdpBroadcastReceiver.cpp
udp 브로드캐스트 수신
#### UdpBroadcastSender.cpp
udp 브로드캐스트 송신

* * *
## Server Develop Info
1. TCP를 사용할 경우 클라이언트 개수만큼 소켓이 있어야 함
2. 파일 핸들을 하는 동안 스레드가 대기하는 일은 없어야 함, 있다면 사용자 입장에서 일시 정지한 것처럼 보여짐
3. 그래서 보통 소켓은 Asynchronous I/O(비동기 입출력) 상태로 다룸
4. 비동기 입출력 방식에는 크게 non-blocking socket(논블로킹 소켓), Overlapped I/O가 있고 이 방식들을 발전시킨 epoll, IOCP(I/O Completion Port) 방식이 있음
### blocking socket
소켓뿐만 아니라 파일 핸들 함수에도 디바이스에 처리 요청을 걸어 놓고 응답을 대기하는 함수를 호출할 때 스레드에서 발생하는 대기 현상을 모두 blocking(블로킹)이라고 함
```
    blocking 일어나는 함수

    connect() 연결 대기시간 동안 발생
    send() 송신 버퍼에 데이터가 가득 차면 발생
    ※ send()는 상대에게 바로 보내는 것이 아니라 송신 버퍼에 보내고 OS가 송신 버퍼에 있는 것은 상대에게 보냄

    accept() blocking중일 때 이 함수를 호출하면 blocking 발생
    recv() 수신할 수 있는 데이터가 없으면, 즉, 수신 버퍼 안에 데이터가 없으면 그 데이터가 있을 때까지 발생
    ※ TCP에서 송신 버퍼에 데이터를 쌓는 속도보다 수신 버퍼에 데이터를 꺼내는 속도가 더 느리면 TCP연결이 끊어지지 않고 느린 쪽에 맞추어 작동함
    ※ UDP에서 송신 버퍼에 데이터를 쌓는 속도보다 수신 버퍼에 데이터를 꺼내는 속도가 더 느리면 UDP 데이터그램은 수신하는 쪽이 모두 버림
```
TCP는 OS가 알아서 초당 송신량을 조절하고 UDP는 그런 것이 없어서 과한 데이터가 간다면 주변의 네트워킹이 두절되는데 이 현상을 혼잡 현상이라 함
### non-blocking socket
OS에서 소켓 함수가 블로킹되지 않게 하는 API를 추가로 제공하는데 이것을 논블록 소켓이라 함   
논블록 소켓을 사용하면 한 스레드에서 여러 소켓을 반복문으로 한꺼번에 다룰 수 있음   
**논블록 소켓 사용 방법**
1. 소켓을 논블록 소켓으로 전환
2. 평소처럼 네트워킹 함수 호출
3. 논블록 소켓은 무조건 즉시 리턴 함, 성공 또는 would block오류 둘 중 하나 리턴
※ 논블록 수신 함수에서 would block오류가 나오면 잠시 후 다시 논블록 수신 함수를 호출하면 됨   
만약 connect()를 논블록으로 사용할 때 would block오류가 나오면 이미 연결 과정이 진행 중인 상태로 변한 거여서 다시 연결을 시도하면 안 됨, 대신 0바이트 송신으로 어떤 상태인지에 따라 처리해야 함
```
    0바이트 송신

    1. 0바이트 송신하는 함수가 성공을 리턴하면 TCP에 연결되어 있다는 의미
    2. ENOTCONN(소켓이 연결되어 있지 않음)을 리턴하면 TCP연결이 진행 중
    3. 기타 오류 코드가 나오면 TCP연결 시도 실패
    
    ※ 클라이언트에서는 무한루프를 돌면서 CPU를 100% 사용하여 에러처리해도 되지만 서버에서는 CPU가 최대한 놀고있어야 함
```
여러 소켓을 반복문으로 사용할 때 한 소켓이라도 블로킹이 일어나면 CPU가 폭주함, 이것을 해결해주는 논블록 함수는 select() 또는 poll() 함수임
```
    select(소켓리스트, 대기시간);
    블록이 일어날 경우 대기시간 동안 기다렸다가 리턴하거나 그 전에라도 조건이 만족되면 즉시 리턴
    ※ OS마다 사용방법이 다름

    poll();
    윈도우에서는 WSAPoll()을 쓰고 리눅스에서는 poll()을 사용함
```
accept()가 논블록 소켓일 경우 TCP연결이 아직 들어오지 않으면 blocking 대신 would block 오류코드를 리턴 함   
그래서 select()나 poll()에서 I/O 가능 이벤트가 감지되면 accept()를 호출하면 됨
```
    논블록 소켓의 장점
    1. 스레드 블로키잉 없어서 중도 취소 같은 통제가 가능
    2. 소켓을 여러 개 다룰 수 있음
    3. 스레드 개수가 적거나 1개이므로 연산량과 호출 스택 메모리가 낭비되지 않음

    논블록 소켓의 단점
    1. would block을 리턴할 경우 재시도 호출 낭비가 발생
    2. 소켓 I/O 함수를 호출할 때 데이터를 버퍼로 옮기는 데이터 블록에 대한 복사 연산이 발생
    3. 소켓 I/O 함수의 재시도 호출에 일관되지 않은 API(connect, send & receive) 문제가 있음
```
### Overlapped I/O or Asynchronous I/O
논블록 소켓의 여러 단점과 재시도용 호출 낭비 문제와 데이터 블록 복사 부하 문제를 모두 해결해 줌
```
    재시도용 호출 낭비 문제
    TCP의 send()와 receive()는 크게 문제 없지만 UDP의 send()처리에 문제가 있음
    UDP에서 송신 데이터가 5bytes이고 수신 버퍼에 5bytes중 4bytes를 계속 차지하고 있으면 CPU가 계속 낭비 됨

    데이터 블록 복사 부하 문제
    send()같은 연산은 CPU의 캐시가 아닌 RAM에서 보낼 데이터를 소켓 버퍼로 복사하기 때문에 매우 느림
    그래서 고성능 서버 개발 불가능
```
그러나 Overlapped I/O도 성능은 유리하지만 문제가 있음
**Overlapped I/O의 문제**
1. 윈도우에서만 사용할 수 있음
2. OS와 Overlapped I/O는  별도로 동시간대에 진행되는데 OS가 Overlapped I/O의 데이터 블록에 백그라운드에서 액세스 하는 것이 문제
3. 즉, 코드가 실행중에 OS가 맘데로 데이터를 건드림(중첩된의 뜻 = overlapped)
4. 따라서 Overlapped I/O 전용 함수가 Asynchronous(비동기)로 하는 일이 완료될 때까지 소켓 API에 인자로 넘긴 데이터 블록을 제거하거나 내용을 변경해선 안 됨
5. 물론 중청해도 되긴 하지만 Overlapped status 객체가 서로 달라야 함
결국 사용하기 어려워서 epoll과 IOCP가 생겨났음
### epoll
epoll은 linux와 android에서만 사용 가능하고 FreeBSD와 MacOS, iOS에서는 epoll과 유사한 kqueue를 사용함   
epoll은 소켓이 I/O 가능 상태가 되면 이를 감지해서 내장된 큐로 사용자에게 알림(event)을 해 줌   
만약 소켓이 10000개라면 이 중에서 I/O 가능이 된 것들만 epoll을 이용해 얻어 루프를 10000번 돌지 않아도 됨
**epoll의 문제**
거의 대부분 소켓의 송신 가능이라 level trigger로 짠다면 불필요한 CPU 연산 낭비가 발생함
```
    ※ 전자공학에서 나온 용어로 추정
    level trigger(레벨 트리거): 소켓이 I/O 가능하다
    I/O 가능인 이상 항상 epoll에서 꺼내어 짐

    edge trigger(에지 트리거): 소켓이 I/O 가능이 아니었는데 이제 가능하게 되었다
    I/O 가능이 아니었다가 가능으로 변하는 순간에만 꺼내어 짐
    ※ edge trigger의 주의사항
    1. UDP에서 receive() 중 영원히 남은 데이터를 못꺼낼 수 있어서 I/O 호출을 한 번만 하지 말고 would block이 발생할 때까지 반복해야 함
    2. 소켓은 논블록으로 미리 설정되어 있어야 함
```
epoll은 connect()와 accept()에서도 I/O 가능 이벤트를 받을 수 있음   
connect()는 send 이벤트와 accept()는 receive 이벤트와 동일하게 취급해서 리스닝 소켓에 대한 receive 이벤트를 받을 경우 accept()를 호출하면 새로운 TCP 연결 소켓을 얻을 수 있음
### IOCP(I/O Completion Port)
Overlapped I/O처럼 윈도우에서만 사용 가능   
IOCP는 소켓의 Overlapped I/O가 완료되면 이를 감지해서 내장된 큐로 사용자에게 알려(event) 줌   
만약 소켓이 10000개라면 이 중에서 I/O가 완료된 것들만 IOCP를 이용해 얻어 루프를 10000번 돌지 않아도 됨
```
    IOCP와 epoll의 비교

    <블로킹을 없애는 수단>
    IOCP            | epoll
    Overlaped I/O   | 논블록 소켓
    
    <블로킹 없는 처리 순서>
    IOCP                               | epoll
    1. Overlaped I/O를 검              | 1. I/O 이벤트를 꺼냄
    2. 완료 신호를 꺼냄                 | 2. 그것에 논블록 I/O를 실행
    3. 완료 신호에 대한 나머지 처리      |
    4. 끝나고 다시 Overlaped I/O를 검   |

    <UDP 처리 - 소켓 하나에 UDP 데이터 2개인 경우>
    IOCP                                                    | epoll
    소켓 하나에 완료 신호를 스레드 하나만 처리하도록 보장 됨    | 동시에 꺼내서 따로 로직 처리가 필요

    ※ IOCP은 epoll보다 사용법이 복잡하지만 성능상 유리한 것이 많음
    ※ 특히 IOCP는 대규모 동시접속자 게임 서버를 만들 때 멀티코어를 모두 활용해야할 때 유리함
```
