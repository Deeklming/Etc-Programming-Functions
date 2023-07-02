# MULTI THREAD
Multi Thread Programming
### PrimeNumber.cpp
멀티 스레드로 뮤텍스를 사용하여 소수 찾기
## Context Switch
하나의 프로세스가 두 개 이상의 스레드를 작업할 때 교대로 실행하면 동시에 실행하는 것 처럼 보임   
그것을 위해 한 스레드의 작업을 잠시 저장했다가 다른 스레드의 저장했었던 작업을 다시 불러와 실행하는 것을 Context Switch(컨텍스트 전환)라고 함   
컨텍스트 전환을 빠르게 하려면 Sleep(0); 을 호출하면 됨

## Windows
윈도우에서는 다른 OS와 다르게 '다른 OS 프로세스 = 윈도우 OS 프로세스 + 스레드'의 형태를 가짐   
윈도우에서 스레드는 높은 우선순위(Priority)를 가진 스레드가 먼저 CPU의 시간을 할당받음
- 프로세스 우선순위: Priority Class(우선순위 클래스)
- 스레드 우선수위: Priority Level(우선순위 레벨)
- Base Priority(기본 우선수위): Priority Class와 Priority Level을 결합하여 윈도우 CPU의 우선순위가 결정되고 스레드 스케줄링에 사용됨
### ThreadBasicWin.cpp
윈도우에서 스레드 사용 기본
### TcpServerMultiThreadWin.cpp
윈도우에서 멀티 스레드 사용 기본   
TcpClientMultiCheckWin.cpp로 여러개 띄워 확인
### Thread Synchronization
윈도우 스레드 동기화 기법
1. Critical Section(임계 영역): 한 프로세스 안에 스레드들 중 공유 자원에 하나의 스레드만 접근 허용
2. Mutex(뮤텍스): 서로 다른 또는 한 프로세스의 스레드들 중 공유 자원에 하나의 스레드만 접근 허용
3. Semaphore(세마포어): 한정된 개수의 공유 자원에 여러 스레드가 접근할 때 스레드 개수를 제한함
4. Event(이벤트): 특정 조건이 만족 되면 신호로 대기 중인 스레드를 깨움
5. Waitable Timer(대기 가능 타이머): 특정 시간에 대기 중인 스레드를 깨움   
※ Critical Section을 제외하고 모두 다른 프로세스에 속한 스레드 간에 사용 가능하고 그것들은 모두 매개체 역할을 하는 Synchronization Object(동기화 객체)임
### ThreadSynchronizationWin.cpp
윈도우에서 임계 영역과 이벤트 스레드 동기화   
※ Critical Section으로 동기화처리 한다면 스레드의 순서를 정할 수 없음   
※ 비신호 상태와 신호 상태의 확인이 중요함
## Linux
리눅스에서 스레드를 사용하려면 시스템 콜을 사용해야 함   
리눅스의 스레드는 Task(태스크)라고 불리는 실행 단위와 일대일 대응 함   
리눅스에서 스레드는 높은 우선순위(Priority)를 가진 태스크가 먼저 CPU의 시간을 할당 받음   
리눅스는 윈도우와 달리 다른 스레드를 일시 정지하거나 재시작하는 함수가 없지만 스레드 동기화 기법으로 비슷한 효과를 낼 수 있음
- Scheduling Policy(스케줄링 정책): Real-time(실시간)과 Normal(정규) 정책이 있음, 정규보다 실시간을 따르는 태스크가 항상 우선순위가 높음
- Scheduling Priority(스케줄링 우선수위): 실시간 스케줄링은 1(최저)~99(최고)까지의 우선순위가 있고 정규 스케줄링은 고정된 순위를 지원하지 않아서 항상 0으로 간주함
### ThreadBasicLix.cpp
리눅스에서 스레드 사용 기본
### TcpServerMultiThreadLix.cpp
리눅스에서 멀티 스레드 사용 기본   
TcpClientMultiCheckLix.cpp로 여러개 띄워 확인
### Thread Synchronization
리눅스 스레드 동기화 기법
1. Mutex(뮤텍스): 공유 자원에 하나의 스레드만 접근 허용
2. Reader-Writer Lock(읽기-쓰기 잠금): 공유 자원에 대해 읽기는 여러 스레드에 허용하지만 쓰기는 한 스레드만 허용
3. Condition Variable(조건 변수): 특정 조건이 만족 되면 신호로 대기 중인 스레드를 깨움
4. Barrier(배리어): 일정 개수의 스레드를 먼저 모두 대기 상태로 만든 후 조건이 만족되면 대기 중인 모든 스레드를 깨움   
※ 서로 다른 프로세스에 속한 스레드 간 동기화는 System V IPC 부류의 시스템 콜을 사용해야 함   
※ 여러 스레드가 동기화하기 위해선 매개체가 필요 함
### ThreadSynchronizationLix.cpp
리눅스에서 뮤텍스와 조건 변수 스레드 동기화
#### linux signal and accept()
리눅스에서 시그널 처리시 accept()함수가 -1(오류의 의미)을 리턴할 수 있음   
그래서 의도치 않게 서버가 종료될 수 있어서 적절한 처리가 필요함
```c
    //Python의 경우 내부로 이런 방식으로 처리함
    while (1) {
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
            if(errno == EINTR) continue;// 시그널로 인해 리턴했다면 루프 처음으로 되돌아감
			err_print_msg("accept()");
			break;
		}
	}
```
