# Network Programming

## Socket
* C

## Socket Echo
### C - windows
**clang++ compile**   
```
common
clang++ -include .\Common.h -c .\Common.cpp

ipv4
clang++ -include .\Common.h -c .\TcpEchoServer4.cpp
clang++ -include .\Common.h -c .\TcpEchoClient4.cpp
clang++ -o ipv4server.exe .\TcpEchoServer4.o .\Common.o
clang++ -o ipv4client.exe .\TcpEchoClient4.o .\Common.o

ipv6
clang++ -include .\Common.h -c .\TcpEchoServer6.cpp
clang++ -include .\Common.h -c .\TcpEchoClient6.cpp
clang++ -o ipv6server.exe .\TcpEchoServer6.o .\Common.o
clang++ -o ipv6client.exe .\TcpEchoClient6.o .\Common.o
```
서버 먼저 실행 후 클라이언트 실행   

**netstat check**   
```
ipv4
netstat -a -n -p tcp | findstr 9000

ipv6
netstat -a -n -p tcpv6 | findstr 9000
```
TIME_WAIT 상태는 보통 5분안에 자동으로 사라짐

## SOCKET_DATA_SEND_WAY
### C - windows
**Fixed**   
고정 길이 통신   

**Variable**   
가변 길이 통신   

**FixedVariable**   
고정 길이 + 가변 길이 통신   

**CloseOnTransfer**   
데이터 전송 후 종료 통신   
