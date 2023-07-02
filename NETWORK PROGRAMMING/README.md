# NETWORK SERVICE PROGRAMMING
Network-related functions

## Use Programming Language
* C
* C++
* JAVASCRIPT

## Use Tools
* Visual Studio Code (VSCode)

## OS Functions
### Common
**network tools**
```
    netstat -ano : 사용중인 모든 포트 확인
    netstat -a -n -p tcp | findstr 9000 : 사용중인 tcp 중 9000이라는 문자열(포트)을 찾아 출력
    netstat -a -n -p tcpv6 | findstr 9000
    netstat -a -n -p udpv6
```
### Linux
**network tools**
```
    ifconfig : 전체 주소 확인
    traceroute domain or ip : 네트워크 경로 확인

    netstat -a -n --udp -6
```
※ if no package - ifconfig
> debian: sudo apt-get install net-tools
> rocky: sudo dnf install net-tools

### Windows
**network tools**
```
    ipconfig /all : 전체 주소 확인
    tracert domain or ip : 네트워크 경로 확인
```
